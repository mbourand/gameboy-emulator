#include "CPU.hpp"
#include "Cartridge.hpp"
#include "Clock.hpp"
#include "Memory.hpp"
#include "PPU.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	std::string path;
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <rom_file>" << std::endl;
		return 1;
	}
	else
		path = argv[1];

	sf::RenderWindow window(sf::VideoMode(160 * 4, 144 * 4), "GBMU");

	gbmu::Cartridge cartridge;
	try
	{
		cartridge.loadROM(path);
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	gbmu::Memory memory;
	memory.loadCartridge(cartridge);

	gbmu::CPU cpu(memory);
	gbmu::PPU ppu(memory);

	sf::Clock framerateClock;

	std::ofstream logFile("../../log.txt");

	std::vector<int> ops;
	sf::Event event;

	int cyc = 0;
	auto time = std::chrono::high_resolution_clock::now();
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (cyc < gbmu::CPU::CPU_FREQUENCY / 60)
		{
			cpu.update_timers();
			if (cpu._cycleTimer == 0)
				logFile << cpu << " LY:" << std::dec << (int)memory.readByte(0xFF44) << std::endl;
			uint8_t op = memory.readByte(cpu.pc);
			if (std::find(ops.begin(), ops.end(), op) == ops.end())
			{
				std::cout << (int)op << std::endl;
				ops.push_back(op);
			}
			cpu.tick();
			ppu.tick();
			cyc++;
		}

		if (framerateClock.getElapsedTime().asMilliseconds() > 1000 / 60.f)
		{
			// std::cout << cyc << std::endl;
			cyc = 0;
			framerateClock.restart();

			const sf::Image& image = ppu.getImage();
			sf::Texture texture;
			if (texture.loadFromImage(image))
			{
				window.clear();
				sf::Sprite sprite;
				sprite.setTexture(texture);
				sprite.setScale({4.f, 4.f});
				window.draw(sprite);
				window.display();
			}
		}
	}

	return 0;
}
