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
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Sprite *Interface::hearts = NULL;
Image *Interface::pad = NULL;
Image *Interface::buttonA = NULL;

void Interface::titleScreen() {
	// Load title screen background
	Image *titleScreen = new Image((char*)"graphics/interface/titleScreen.png");
	
	// Wait for player
	SDL_Event event;
	bool cont;
	while(cont) {
		// Close window: exit game
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
				case SDL_QUIT: cont = false;
			}
		};
		
		// Clear main window
		Game::MainWindow->clear();
		
		// Display background
		titleScreen->render();
		
		// Update main window display
		Game::MainWindow->update();
	}
	
	// Delete title screen image
	delete titleScreen;
}

void Interface::initialize() {
	// Load hearts sprite
	hearts = new Sprite((char*)"graphics/interface/hearts.png");
	
	// Load pad image
	pad = new Image((char*)"graphics/interface/pad.png");
	pad->setAlpha(175);
	
	// Load button images
	buttonA = new Image((char*)"graphics/interface/a.png");
	buttonA->setAlpha(175);
}

void Interface::unload() {
	// Delete images
	delete hearts;
	delete pad;
	delete buttonA;
}

void Interface::renderHUD() {
	// Get viewport position
	s16 x = Game::MainWindow->viewportX();
	s16 y = Game::MainWindow->viewportY();
	
#ifdef PAD
	// Render pad
	pad->setPosRect(Game::MainWindow->viewportX() + 16, Game::MainWindow->viewportY() + Game::MainWindow->viewportH() - pad->height() - 16, pad->width(), pad->height());
	pad->render();
	
	// Render buttons
	buttonA->setPosRect(Game::MainWindow->viewportX() + Game::MainWindow->viewportW() - buttonA->width() - 16, Game::MainWindow->viewportY() + Game::MainWindow->viewportH() - buttonA->height() - 16, buttonA->width(), buttonA->height());
	buttonA->render();
#endif
	
	// Get heart data
	double entireHearts = 0;
	double piecesOfHearts = modf((double)CharacterManager::player()->lifes() / 4, &entireHearts);
	
	// Render hearts
	for(u8 i = 0 ; i < CharacterManager::player()->maxLifes() / 4 ; i++) {
		if(i < entireHearts) hearts->drawFrame(x + 16 * i, y, 4);
		else if(i == entireHearts + ceil(piecesOfHearts) - 1) hearts->drawFrame(x + 16 * i, y, ((i < entireHearts + ceil(piecesOfHearts) - 1) ? 4 : piecesOfHearts * 4));
		else if(i > entireHearts + ceil(piecesOfHearts) - 1) hearts->drawFrame(x + 16 * i, y, 0);
	}
}

void Interface::renderMonsterLife(Monster *monster) {
	// Render life bar
	Game::MainWindow->drawFillRect(monster->x() - 1, monster->y() - 6, 18, 4, Color::black);
	Game::MainWindow->drawFillRect(monster->x(), monster->y() - 5, 16, 2, Color::white);
	Game::MainWindow->drawFillRect(monster->x(), monster->y() - 5, monster->lifes() * 16 / monster->maxLifes(), 2, Color::life);
}

void Interface::newDialogBox(char *text) {
/*	
	// Initialize box
	sf::RectangleShape box;
	box.setPosition((MAP_WIDTH * 16) / 8, 3 * (MAP_HEIGHT * 16) / 4);
	box.setSize(sf::Vector2f(6 * (MAP_WIDTH * 16) / 8, (MAP_HEIGHT * 16) / 4));
	box.setFillColor(sf::Color::Black);
	sf::Text pressKeyText((char*)"Press Z key to continue.", *Game::defaultFont, 12);
	pressKeyText.setPosition(10 * MAP_WIDTH + 28, (MAP_HEIGHT - 1) * 16);
	
	// Initialize string
	sf::Text string(text, *Game::defaultFont);
	string.setPosition((MAP_WIDTH * 16) / 8 + 8, 3 * (MAP_HEIGHT * 16) / 4 + 8);
	
	// Render string
	Game::MainWindow->setView(*Sprite::View);
	Game::MainWindow->draw(box);
	Game::MainWindow->draw(pressKeyText);
	Game::MainWindow->draw(string);
	Game::MainWindow->setView(Game::MainWindow->getDefaultView());
	Game::MainWindow->display();
	
	// Wait for Z pressed
	sf::Event event;
	while(Game::MainWindow->isOpen() && !(Game::MainWindow->pollEvent(event) && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)) {
		// Close window: exit game
		if((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
			Game::MainWindow->close();
		}
	}
	
	// Reset view
	Game::MainWindow->setView(Game::MainWindow->getDefaultView());	
*/
}

