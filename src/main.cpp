#include "CPU.hpp"
#include "Cartridge.hpp"
#include "Clock.hpp"
#include "Memory.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <rom_file>" << std::endl;
		return 1;
	}

	sf::RenderWindow window(sf::VideoMode(640, 480), "GBMU");
	window.setFramerateLimit(60);

	gbmu::Cartridge cartridge;
	try
	{
		cartridge.loadROM(argv[1]);
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	gbmu::Memory memory;
	memory.loadCartridge(cartridge);

	gbmu::CPU cpu(memory);
	for (int i = 0; i < 12350; i++)
	{
		if (12350 - i < 100)
			std::cout << cpu << std::endl;
		cpu.tick();
	}

	gbmu::Clock cpuClock(gbmu::CPU::CPU_FREQUENCY);
	sf::Clock framerateClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		cpu.update_timers();

		if (cpuClock.isTimeout())
		{
			cpuClock.reset();
			cpu.tick();
		}

		if (framerateClock.getElapsedTime().asMilliseconds() > 1 / 60.f)
		{
			framerateClock.restart();
			window.clear();
			window.display();
		}
	}

	return 0;
}
