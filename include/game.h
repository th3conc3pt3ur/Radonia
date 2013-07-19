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
#ifndef GAME_H
#define GAME_H

class Game {
	public:
		// Constructor and destructor
		Game();
		~Game();
		
		// Game main loop
		void mainLoop();
		
		// Test scroll func
		void scroll();
		
		// Main window
		static sf::RenderWindow *MainWindow;
		static const sf::Input *Input;
		
		// Current map
		static Map *currentMap;
		
		// Player
		static Player *player;
		
	private:
		// Main loop states
		bool m_continue;
		bool m_paused;
		
		// Overworld maps array
		Map **m_maps;
};

#endif // GAME_H
