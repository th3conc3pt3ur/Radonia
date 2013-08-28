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
#include "weapon.h"
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

Image::Image(const char *filename) {
	// Set class members
	m_filename = filename;
	
	// Initialize image
	SDL_RWops *file = SDL_RWFromFile(filename, "rb");
	m_surface = IMG_Load_RW(file, 1);
	
	if(!m_surface) {
		fprintf(stderr, "Failed to load image \"%s\": %s\n", filename, IMG_GetError());
		exit(EXIT_FAILURE);
	}
	
	// Get dimensions
	m_width = m_surface->w;
	m_height = m_surface->h;
	
	// Initialize texture
	m_texture = SDL_CreateTextureFromSurface(Game::MainWindow->renderer(), m_surface);
	if(!m_texture) {
		fprintf(stderr, "Failed to create texture from image: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	// Initialize rects
	m_clipRect.x = 0;
	m_clipRect.y = 0;
	m_clipRect.w = m_width;
	m_clipRect.h = m_height;
	
	m_posRect.x = 0;
	m_posRect.y = 0;
	m_posRect.w = m_width;
	m_posRect.h = m_height;
}

Image::~Image() {
	SDL_FreeSurface(m_surface);
	SDL_DestroyTexture(m_texture);
}

void Image::render() {
	SDL_RenderCopy(Game::MainWindow->renderer(), m_texture, &m_clipRect, &m_posRect);
}

void Image::setPosRect(u16 x, u16 y, u16 w, u16 h) {
	m_posRect.x = x;
	m_posRect.y = y;
	m_posRect.w = w;
	m_posRect.h = h;
}

void Image::setClipRect(u16 x, u16 y, u16 w, u16 h) {
	m_clipRect.x = x;
	m_clipRect.y = y;
	m_clipRect.w = w;
	m_clipRect.h = h;
}

