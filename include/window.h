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
		
		// Setup window icon
		void setupIcon();
		
		// Update window
		void update(bool viewport = true);
		
		// Clear the window
		void clear();
		
		// Set renderer color
		void setRendererColor(Color color);
		
		// Draw rect
		void drawRect(s16 x, s16 y, u16 w, u16 h, Color c);
		
		// Draw fill rect
		void drawFillRect(s16 x, s16 y, u16 w, u16 h, Color c);
		
		// Update viewport position
		void updateViewportPosition(s16 x, s16 y);
		
		// Center viewport with object
		void centerViewportWithObject(s16 x, s16 y, u16 w, u16 h);
		
		// Get SDL_Window object
		SDL_Window *window() { return m_window; }
		
		// Get SDL_Renderer object
		SDL_Renderer *renderer() { return m_renderer; }
		
		// Get viewport position
		s16 viewportX() const { return m_viewportX; }
		s16 viewportY() const { return m_viewportY; }
		
		// Get viewport size
		u16 viewportW() const { return m_viewportW; }
		u16 viewportH() const { return m_viewportH; }
		
	private:
		// SDL_Window object
		SDL_Window *m_window;
		
		// SDL_Renderer object
		SDL_Renderer *m_renderer;
		
		// Window icon
		SDL_Surface *m_icon;
		
		// Window caption
		char *m_caption;
		
		// Window size
		u16 m_width;
		u16 m_height;
		
		// Viewport position
		s16 m_viewportX;
		s16 m_viewportY;
		
		// Viewport size
		u16 m_viewportW;
		u16 m_viewportH;
		
};

#endif // WINDOW_H
