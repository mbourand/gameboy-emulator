#include "CPU.hpp"
#include "Cartridge.hpp"
#include "Clock.hpp"
#include "Gameboy.hpp"
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
	gbmu::Gameboy gb(cartridge);
	gb.run();

	return 0;
}
