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
#include "font.h"
#include "sound.h"
#include "net.h"
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
#include "mapManager.h"
#include "doorManager.h"
#include "animationManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Font::Font(const char *filename) {
	// Initialize fonts
	m_fontSmall  = TTF_OpenFont(filename, FONT_SMALL);
	m_fontMedium = TTF_OpenFont(filename, FONT_MEDIUM);
	m_fontLarge  = TTF_OpenFont(filename, FONT_LARGE);
}

Font::~Font() {
	// Destroy fonts
	TTF_CloseFont(m_fontSmall);
	TTF_CloseFont(m_fontMedium);
	TTF_CloseFont(m_fontLarge);
}

void Font::print(const char *str, u16 x, u16 y, FontSize size, Color color) {
	// Initialize temporary font variable
	TTF_Font *font = NULL;
	
	// Get font size
	switch(size) {
		case FONT_SMALL:  font = m_fontSmall;  break;
		case FONT_MEDIUM: font = m_fontMedium; break;
		case FONT_LARGE:  font = m_fontLarge;  break;
	}
	
	// Initialize texture
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, str, SDL_Color{color.r, color.g, color.b});
	Image textToDisplay(textSurface);
	
	// Render text
	textToDisplay.setPosRect(x, y, textToDisplay.width(), textToDisplay.height());
	textToDisplay.render();
}

void Font::printTextBox(const char *str, u16 x, u16 y, u16 width, u16 height, FontSize size, Color color) {
}

