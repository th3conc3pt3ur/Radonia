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
#ifndef WINDOW_H
#define WINDOW_H

class Window {
	public:
		// Constructor and destructor
		Window(char *caption, u16 width, u16 height);
		~Window();
		
		// Update window
		void update();
		
		// Clear the window
		void clear();
		
		// Set renderer color
		void setRendererColor(Color color);
		
		// Draw rect
		void drawRect(s16 x, s16 y, u16 w, u16 h, Color c);
		
		// Draw fill rect
		void drawFillRect(s16 x, s16 y, u16 w, u16 h, Color c);
		
		// Get SDL_Window object
		SDL_Window *window() { return m_window; }
		
		// Get SDL_Renderer object
		SDL_Renderer *renderer() { return m_renderer; }
		
		// Get SDL_Surface object
		SDL_Surface *surface() { return SDL_GetWindowSurface(m_window);}
		
	private:
		// SDL_Window object
		SDL_Window *m_window;
		
		// SDL_Renderer object
		SDL_Renderer *m_renderer;
		
		// Window caption
		char *m_caption;
		
		// Window size
		u16 m_width;
		u16 m_height;
		
};

#endif // WINDOW_H
