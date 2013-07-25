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
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"

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

Sprite::Sprite(char *filename, u8 frameSize) {
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
}

Sprite::~Sprite() {
}

void Sprite::drawFrame(s16 x, s16 y, int frame) {
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

void Sprite::playAnimation(s16 x, s16 y, int anim) {
	// If the animation is not playing
	if(!m_animations[anim]->isPlaying()) {
		resetAnimation(anim); // Reset animation timer
		startAnimation(anim); // Start animation timer
		m_animations[anim]->isPlaying(true); // Set isPlaying boolean to true
	}
	
	// If the animation is at end
	if(m_animations[anim]->tmr()->time() / m_animations[anim]->delay() >= m_animations[anim]->size()) {
		resetAnimation(anim); // Reset animation timer
		startAnimation(anim); // Start animation timer
	}
	
	// This variable contains the number of the animation's frame to draw
	int animToDraw = m_animations[anim]->tabAnim()[(int)(m_animations[anim]->tmr()->time() / m_animations[anim]->delay())];
	drawFrame(x, y, animToDraw); // Draw the frame
}

