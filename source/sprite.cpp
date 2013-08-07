/*---------------------------------------------------------------------------------
	
	Radonia
	Copyright (C) 2013 Deloptia <deloptia.devteam@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
---------------------------------------------------------------------------------*/
#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "timer.h"
#include "sprite.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"
#include "tools.h"
#include "collisions.h"

Sprite_Animation::Sprite_Animation(int size, int *tabAnim, int delay) {
	// Set class members
	m_size = size;
	m_tabAnim = tabAnim;
	m_delay = delay;
	m_tmr = new Timer();
	m_isPlaying = false;
}

Sprite_Animation::~Sprite_Animation() {
	// Delete timer
	delete m_tmr;
}

sf::View *Sprite::View = NULL;

Sprite::Sprite(char *filename, SpriteType type, s16 x, s16 y, u8 frameSize) {
	// Initialize sprite view
	if(View == NULL) View = new sf::View(sf::FloatRect(0, 0, 640, 480));
	
	// Load sprite
	sf::Image img;
	img.loadFromFile(filename);
	img.createMaskFromColor(sf::Color(255, 0, 255));
	
	m_tex.loadFromImage(img);
	
	m_spr.setTexture(m_tex);
	
	// Set frame size
	m_frameSize = frameSize;
	
	// Set default color
	m_defaultColor = m_spr.getColor();
	
	// Setup hurt timer
	m_hurtTimer.reset();
	m_hurtTimer.start();
	
	m_timerLastValue = 0;
	
	// Set type
	m_type = type;
	
	// Set position
	m_x = x;
	m_y = y;
	
	// Set movement vectors
	m_vx = 0;
	m_vy = 0;
	
	// Reset collision states
	collidedSprite = NULL;
	collidedTile = 0;
}

Sprite::~Sprite() {
}

void Sprite::drawFrame(s16 x, s16 y, int frame) {
	// Update position
	m_x = x;
	m_y = y;
	
	// Get frame position
	u16 frameY = (frame / (m_tex.getSize().x / m_frameSize)) * m_frameSize;
	u16 frameX = (frame - (frameY / m_frameSize) * (m_tex.getSize().x / m_frameSize)) * m_frameSize;
	
	// Setup sprite
	m_spr.setPosition(x, y);
	m_spr.setTextureRect(sf::IntRect(frameX, frameY, m_frameSize, m_frameSize));
	
	// Set view for drawing sprites
	Game::MainWindow->setView(*View);
	
	// Draw the sprite
	Game::MainWindow->draw(m_spr);
	
	// Reset the view
	Game::MainWindow->setView(Game::MainWindow->getDefaultView());
}

void Sprite::addAnimation(int size, int *tabAnim, int delay) {
	Sprite_Animation* tmp = new Sprite_Animation(size, tabAnim, delay);
	m_animations.push_back(tmp);
}

void Sprite::resetAnimation(int anim) {
	m_animations[anim]->tmr()->reset();
}

void Sprite::startAnimation(int anim) {
	m_animations[anim]->tmr()->start();
}

void Sprite::stopAnimation(int anim) {
	m_animations[anim]->tmr()->stop();
}

bool Sprite::animationAtEnd(int anim) {
	return m_animations[anim]->tmr()->time() / m_animations[anim]->delay() >= m_animations[anim]->size();
}

bool Sprite::animationAtFrame(int anim, int frame) {
	return (int)(m_animations[anim]->tmr()->time() / m_animations[anim]->delay()) == frame;
}

void Sprite::playAnimation(s16 x, s16 y, int anim) {
	// If the animation is not playing
	if(!m_animations[anim]->isPlaying()) {
		resetAnimation(anim); // Reset animation timer
		startAnimation(anim); // Start animation timer
		m_animations[anim]->isPlaying(true); // Set isPlaying boolean to true
	}
	
	// If the animation is at end
	if(animationAtEnd(anim)) {
		resetAnimation(anim); // Reset animation timer
		startAnimation(anim); // Start animation timer
	}
	
	// This variable contains the number of the animation's frame to draw
	int animToDraw = m_animations[anim]->tabAnim()[(int)(m_animations[anim]->tmr()->time() / m_animations[anim]->delay())];
	drawFrame(x, y, animToDraw); // Draw the frame
}

void Sprite::hurt() {
	if((collidedSprite && (collidedSprite->isPWeapon() && isMonster()))
	|| (collidedSprite && ((collidedSprite->isMWeapon() || collidedSprite->isMonster()) && isPlayer()))) {
		if(m_hurtTimer.time() - m_timerLastValue > 5) {
			if(isPlayer()) {
				// Block comamnds
				m_blockedCommands = true;
			} else {
				// Stop monster movement
				m_movementTimer.stop();
			}
			
			// Change sprite texture
			sf::Color c = m_spr.getColor();
			m_spr.setColor(invertColor(c));
			
			// Get sprite direction vectors
			s8 e_x = m_x - collidedSprite->x();
			s8 e_y = m_y - collidedSprite->y();
			
			// Set movement vectors
			if(!collidedTile && abs(e_x) > 8) m_vx = (e_x==0)?0:((e_x<0)?-2:2);
			if(!collidedTile && abs(e_y) > 8) m_vy = (e_y==0)?0:((e_y<0)?-2:2);
			
			// Temporary collided sprite
			Sprite *tmpCollidedSprite = collidedSprite;
			
			// Reset collide sprite
			collidedSprite = NULL;
			
			// Test collisions
			testCollisions();
			
			// Reset collided sprite with temp value
			collidedSprite = tmpCollidedSprite;
			
			// Reset timer last value
			m_timerLastValue = m_hurtTimer.time();
			
			// Reset collided sprite and blocked commands states
			if(abs(e_x) > 24 || abs(e_y) > 24 || collidedTile) {
				collidedSprite = NULL;
				if(isPlayer()) m_blockedCommands = false;
				else		   m_movementTimer.start();
				m_spr.setColor(m_defaultColor);
			}
		}
		
		if(m_hurtTimer.time() > 500) {
			// Hurt sprite
			m_lifes--;
			
			// Reset timer
			m_hurtTimer.reset();
			m_hurtTimer.start();
			
			// Reset timer last value
			m_timerLastValue = m_hurtTimer.time();
		}
	}
}

void Sprite::testCollisions() {
	// 0: Right | 1: Left | 2: Up | 3:Down
	for(u8 i = 0 ; i < 4 ; i++) {
		if(((i==0)?(m_vx > 0):((i==1)?(m_vx < 0):((i==2)?(m_vy < 0):(m_vy > 0))))
		&& (!passable(this, m_x + collisionMatrix[i][0], m_y + collisionMatrix[i][1])
		 || !passable(this, m_x + collisionMatrix[i][2], m_y + collisionMatrix[i][3]))) {
			// Reset movement vector
			if(i<2) m_vx = 0;
			else	m_vy = 0;
			
			// Obstacles
			if( passable(this, m_x + collisionMatrix[i][2], m_y + collisionMatrix[i][3])
			&& !passable(this, m_x + collisionMatrix[i][0], m_y + collisionMatrix[i][1])) {
				if((i<2)?(m_vy == 0):(m_vx == 0) && !collidedSprite) {
					if(i<2)	m_vy = 1;
					else	m_vx = 1;
				}
			}
			if( passable(this, m_x + collisionMatrix[i][0], m_y + collisionMatrix[i][1])
			&& !passable(this, m_x + collisionMatrix[i][2], m_y + collisionMatrix[i][3])) {
				if((i<2)?(m_vy == 0):(m_vx == 0) && !collidedSprite) {
					if(i<2) m_vy = -1;
					else	m_vx = -1;
				}
			}
		}
	}
}

