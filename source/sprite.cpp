/*---------------------------------------------------------------------------------
	
	Radonia
	Copyright (C) 2013-2014 Deloptia <deloptia.devteam@gmail.com>
	
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

#include "includeSDL.h"

#include "types.h"
#include "color.h"
#include "config.h"
#include "window.h"
#include "keyboard.h"
#include "timer.h"
#include "image.h"
#include "animation.h"
#include "sprite.h"
#include "character.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "door.h"
#include "animationManager.h"
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Sprite::Sprite(char *filename, u8 frameSize) : Image(filename) {
	// Set frame size
	m_frameSize = frameSize;
	
	m_posRect.w = m_frameSize;
	m_posRect.h = m_frameSize;
}

Sprite::~Sprite() {
}

void Sprite::drawFrame(s16 x, s16 y, u16 frame) {
	// Get frame position
	u16 frameY = (frame / (m_width / m_frameSize)) * m_frameSize;
	u16 frameX = (frame - (frameY / m_frameSize) * (m_width / m_frameSize)) * m_frameSize;
	
	// Set position
	m_posRect.x = x;
	m_posRect.y = y;
	
	// Set clip rect
	m_clipRect.x = frameX;
	m_clipRect.y = frameY;
	m_clipRect.w = m_frameSize;
	m_clipRect.h = m_frameSize;
	
	// Render clipped image
	render();
}

void Sprite::addAnimation(u16 size, u16 *tabAnim, u16 delay) {
	Animation* tmp = new Animation(size, tabAnim, delay);
	m_animations.push_back(tmp);
}

void Sprite::resetAnimation(u16 anim) {
	m_animations[anim]->timer.reset();
}

void Sprite::startAnimation(u16 anim) {
	m_animations[anim]->timer.start();
}

void Sprite::stopAnimation(u16 anim) {
	m_animations[anim]->timer.stop();
}

bool Sprite::animationAtEnd(u16 anim) {
	return m_animations[anim]->timer.time() / m_animations[anim]->delay >= m_animations[anim]->size;
}

bool Sprite::animationAtFrame(u16 anim, u16 frame) {
	return (u16)(m_animations[anim]->timer.time() / m_animations[anim]->delay) == frame;
}

void Sprite::playAnimation(s16 x, s16 y, u16 anim) {
	// If the animation is not playing
	if(!m_animations[anim]->isPlaying) {
		resetAnimation(anim); // Reset animation timer
		startAnimation(anim); // Start animation timer
		m_animations[anim]->isPlaying = true; // Set isPlaying boolean to true
	}
	
	// If the animation is at end
	if(animationAtEnd(anim)) {
		resetAnimation(anim); // Reset animation timer
		startAnimation(anim); // Start animation timer
	}
	
	// This variable contains the number of the animation's frame to draw
	u16 animToDraw = m_animations[anim]->tabAnim[(u16)(m_animations[anim]->timer.time() / m_animations[anim]->delay)];
	drawFrame(x, y, animToDraw); // Draw the frame
}

