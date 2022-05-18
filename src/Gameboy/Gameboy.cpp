#include "Gameboy.hpp"
#include <fstream>

namespace gbmu
{
	Gameboy::Gameboy(Cartridge& cartridge)
		: _cpu(CPU(*this)), _ppu(PPU(*this)), _cartridge(cartridge), _isRunning(false)
	{
		this->_memory.loadCartridge(cartridge);
	}

	uint8_t Gameboy::readByte(uint16_t address)
	{
		if (!_memory.readByte(0xFF50) && address < BOOT_ROM.size())
			return BOOT_ROM[address];
		if (address == Memory::JOYPAD)
			return this->_joypad.getJoypadState(_memory.readByte(Memory::JOYPAD));
		return this->_memory.readByte(address);
	}

	void Gameboy::writeByte(uint16_t address, uint8_t value)
	{
		if (address == Memory::DIV)
			value = 0;
		else if (address == Memory::JOYPAD)
			value = (_memory.readByte(Memory::JOYPAD) & 0b00001111) | (value & 0b00110000);
		this->_memory.writeByte(address, value);
	}

	uint16_t Gameboy::readWord(uint16_t address)
	{
		if (!_memory.readByte(0xFF50) && address < BOOT_ROM.size())
			return (BOOT_ROM[address + 1] << 8) | BOOT_ROM[address];
		return this->_memory.readWord(address);
	}

	void Gameboy::writeWord(uint16_t address, uint16_t value)
	{
		if (address == Memory::DIV)
			value = 0;
		else if (address == Memory::JOYPAD)
			value = (_memory.readByte(Memory::JOYPAD) & 0b00001111) | (value & 0b00110000);
		this->_memory.writeWord(address, value);
	}

	void Gameboy::setDivTimer(uint16_t value) { this->_memory.setDivTimer(value); }

	void Gameboy::run()
	{
		sf::RenderWindow window(sf::VideoMode(160 * 4, 144 * 4), "GBMU");
		sf::Clock framerateClock;
		std::ofstream logFile("../../log.txt");
		sf::Event event;
		int cyc = 0;

		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			while (cyc < gbmu::CPU::CPU_FREQUENCY / 59.7)
			{
				/*if (memory.readByte(0xFF50) & 0x01)
					logFile << cpu << " LY:" << std::dec << (int)memory.readByte(0xFF44) << std::endl;*/
				_cpu.update_timers();
				cyc += _cpu.tick();
				_ppu.tick();
				/*			if (cpu.registers[gbmu::CPU::Reg::B] == 3 && cpu.registers[gbmu::CPU::Reg::C] == 5 &&
								cpu.registers[gbmu::CPU::Reg::D] == 8 && cpu.registers[gbmu::CPU::Reg::E] == 13 &&
								cpu.registers[gbmu::CPU::Reg::H] == 21 && cpu.registers[gbmu::CPU::Reg::L] == 34)
								std::cout << "OK" << std::endl;*/
			}

			if (framerateClock.getElapsedTime().asMilliseconds() > 1000 / 59.7f)
			{
				// std::cout << cyc << std::endl;
				cyc = 0;
				_joypad.update();
				framerateClock.restart();

				const sf::Image& image = _ppu.getImage();
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
	}
}
