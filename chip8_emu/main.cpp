/********************************************************************
** Tier 1 Emu project
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

chip8 daleChip8;
int modifier = 10;

unsigned int display_width = SCREEN_WIDTH;// *modifier;
unsigned int display_height = SCREEN_HEIGHT;// *modifier;

void keyboardDown();
void keyboardUp(unsigned char key, int x, int y);

typedef unsigned __int8 u8;
u8 screenPlacement[SCREEN_HEIGHT][SCREEN_WIDTH][3]; //new data structure, uses 8 bits to quickly transfer data in kernel level programming

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: daleChip8.exe chip8application\n\n");
		return 1;
	}
	//setup window
	sf::RenderWindow window(sf::VideoMode(display_width, display_height), "SFML works!");
	//window.setVerticalSyncEnabled(true);
	// setup input, clear memory registers and screen
	// load software into memory
	if (!daleChip8.loadApplication(argv[1])) {
		printf("failure to load program\n");
		printf("argv[1] : %s\n", argv[1]);
		system("pause");
		return 3;
	}
	//emulation loop
	//printf("In emulation loop");
	sf::Image image;
	image.create(display_width, display_height, sf::Color::Black);
	sf::Sprite sprite;
	sf::Texture texture;
	sf::IntRect area = sf::IntRect();

	while (window.isOpen())
	{
		
		//Emulate a cycle
		daleChip8.emulateCycle();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				exit(0);
			}
		}

		//If draw flag is set, update screen
		if (daleChip8.drawFlag)
		{
			//drawGraphics();
			// Update pixels
			for (int y = 0; y < 32; ++y)
				for (int x = 0; x < 64; ++x)
					if (daleChip8.gfx[(y * 64) + x] == 0)
						screenPlacement[y][x][0] = screenPlacement[y][x][1] = screenPlacement[y][x][2] = 0;	// Disabled
					else
						screenPlacement[y][x][0] = screenPlacement[y][x][1] = screenPlacement[y][x][2] = 255;  // Enabled

			for (int y = 0; y < 32; y++)
			{
				for (int x = 0; x < 64; x++)
				{
					if (screenPlacement[y][x][0] == 255)
						image.setPixel(x, y, sf::Color::White);
					else
						image.setPixel(x, y, sf::Color::Black);
				}
			}

			if (!texture.loadFromImage(image, area)) {
				printf("error with loadFromImage()\n");
				exit(2);
			}
		}

		//Store key press state (Press and Release)
		// Reset key state
		for (int i = 0; i < 16; ++i)
			daleChip8.key[i] = 0;

		keyboardDown();

		sf::Vector2f scale = sprite.getScale();
		//sprite.setScale(scale.x * modifier, scale.y * modifier);
		
		sprite.setTexture(texture, false); // You can redraw the texture if there is a new texture, you just gotta set the bool later
		//sprite.scale(2,2);
		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}



void keyboardDown()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))    // esc
		exit(0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))		
		daleChip8.key[0x1] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))	
		daleChip8.key[0x2] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))	
		daleChip8.key[0x3] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))	
		daleChip8.key[0xC] = 1;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))	
		daleChip8.key[0x4] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))	
		daleChip8.key[0x5] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))	
		daleChip8.key[0x6] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))	
		daleChip8.key[0xD] = 1;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))	
		daleChip8.key[0x7] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))	
		daleChip8.key[0x8] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))	
		daleChip8.key[0x9] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))	
		daleChip8.key[0xE] = 1;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))	
		daleChip8.key[0xA] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))	
		daleChip8.key[0x0] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))	
		daleChip8.key[0xB] = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))	
		daleChip8.key[0xF] = 1;
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (key == '1')		daleChip8.key[0x1] = 0;
	else if (key == '2')	daleChip8.key[0x2] = 0;
	else if (key == '3')	daleChip8.key[0x3] = 0;
	else if (key == '4')	daleChip8.key[0xC] = 0;

	else if (key == 'q')	daleChip8.key[0x4] = 0;
	else if (key == 'w')	daleChip8.key[0x5] = 0;
	else if (key == 'e')	daleChip8.key[0x6] = 0;
	else if (key == 'r')	daleChip8.key[0xD] = 0;

	else if (key == 'a')	daleChip8.key[0x7] = 0;
	else if (key == 's')	daleChip8.key[0x8] = 0;
	else if (key == 'd')	daleChip8.key[0x9] = 0;
	else if (key == 'f')	daleChip8.key[0xE] = 0;

	else if (key == 'z')	daleChip8.key[0xA] = 0;
	else if (key == 'x')	daleChip8.key[0x0] = 0;
	else if (key == 'c')	daleChip8.key[0xB] = 0;
	else if (key == 'v')	daleChip8.key[0xF] = 0;
}