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
#ifndef IMAGE_H
#define IMAGE_H

class Image {
	public:
		// Constructor and destructor
		Image(char *filename);
		~Image();
		
		// Render
		void render();
		
		// Get image size
		u16 width() const { return m_width; }
		u16 height() const { return m_height; }
		
		// Get position
		SDL_Rect *posRect() { return &m_posRect; }
		
		// Set color
		void setColor(Color color) { SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b); }
		
		// Set alpha
		void setAlpha(u8 alpha) { SDL_SetTextureAlphaMod(m_texture, alpha); }
		
		// Set positon
		void setPosRect(u16 x, u16 y, u16 w, u16 h);
		
		// Set clip rect
		void setClipRect(u16 x, u16 y, u16 w, u16 h);
		
	protected:
		// Image filename
		char *m_filename;
		
		// Size
		u16 m_width;
		u16 m_height;
		
		// Texture object
		SDL_Texture *m_texture;
		
		// SDL rects
		SDL_Rect m_clipRect;
		SDL_Rect m_posRect;
		
};

#endif // IMAGE_H
