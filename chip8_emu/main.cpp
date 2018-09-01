#include <SFML/Graphics.hpp>
#include "chip8.h"

chip8 myChip8;

int main()
{
	//setup window
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	//setup input, clear memory registers and screen

	// load game into memory
	for (int i = 0; i < bufferSize; ++i) {
		memory[i + 512] = buffer[i];
	}
	//emulation loop
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
		window.draw(shape);
		window.display();
	}

	return 0;
}