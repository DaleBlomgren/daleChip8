/********************************************************************
** Project description
** -------------------
** Name: daleChip8
**
** Author: Laurence Muller(Chip8 implementation), Dale Blomgren(SFML implementation)
** 
** License: GNU General Public License (GPL) v2 
** ( http://www.gnu.org/licenses/old-licenses/gpl-2.0.html )
**
*********************************************************************/


#include <SFML/Graphics.hpp>
#include "chip8.h"


#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

chip8 myChip8;
int modifier = 10;

int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;


int main()
{
	//setup window
	sf::RenderWindow window(sf::VideoMode(display_width, display_height), "SFML works!");
	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	//setup input, clear memory registers and screen

	// load game into memory
	
	//emulation loop
	sf::Image image;
	image.create(display_width, display_height, sf::Color::White);
	while (window.isOpen())
	{
		//Emulate one cycle
		//myChip8.emulateCycle();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//If draw flag is set, update screen
		if (myChip8.drawFlag)
		{
			//drawGraphics();
		}

		//Store key press state (Press and Release)
		//myChip8.setKeys();

		window.clear();
		//window.draw(shape);
		window.display();
	}

	return 0;
}



void keyboardDown(unsigned char key, int x, int y)
{
	if (key == 27)    // esc
		exit(0);

	if (key == '1')		myChip8.key[0x1] = 1;
	else if (key == '2')	myChip8.key[0x2] = 1;
	else if (key == '3')	myChip8.key[0x3] = 1;
	else if (key == '4')	myChip8.key[0xC] = 1;

	else if (key == 'q')	myChip8.key[0x4] = 1;
	else if (key == 'w')	myChip8.key[0x5] = 1;
	else if (key == 'e')	myChip8.key[0x6] = 1;
	else if (key == 'r')	myChip8.key[0xD] = 1;

	else if (key == 'a')	myChip8.key[0x7] = 1;
	else if (key == 's')	myChip8.key[0x8] = 1;
	else if (key == 'd')	myChip8.key[0x9] = 1;
	else if (key == 'f')	myChip8.key[0xE] = 1;

	else if (key == 'z')	myChip8.key[0xA] = 1;
	else if (key == 'x')	myChip8.key[0x0] = 1;
	else if (key == 'c')	myChip8.key[0xB] = 1;
	else if (key == 'v')	myChip8.key[0xF] = 1;
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (key == '1')		myChip8.key[0x1] = 0;
	else if (key == '2')	myChip8.key[0x2] = 0;
	else if (key == '3')	myChip8.key[0x3] = 0;
	else if (key == '4')	myChip8.key[0xC] = 0;

	else if (key == 'q')	myChip8.key[0x4] = 0;
	else if (key == 'w')	myChip8.key[0x5] = 0;
	else if (key == 'e')	myChip8.key[0x6] = 0;
	else if (key == 'r')	myChip8.key[0xD] = 0;

	else if (key == 'a')	myChip8.key[0x7] = 0;
	else if (key == 's')	myChip8.key[0x8] = 0;
	else if (key == 'd')	myChip8.key[0x9] = 0;
	else if (key == 'f')	myChip8.key[0xE] = 0;

	else if (key == 'z')	myChip8.key[0xA] = 0;
	else if (key == 'x')	myChip8.key[0x0] = 0;
	else if (key == 'c')	myChip8.key[0xB] = 0;
	else if (key == 'v')	myChip8.key[0xF] = 0;
}