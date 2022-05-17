#include "Joypad.hpp"
#include "Memory.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

namespace gbmu
{
	void Joypad::update()
	{
		static sf::Keyboard::Key buttons[8] = {
			sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down,
			sf::Keyboard::X,     sf::Keyboard::Z,    sf::Keyboard::V,  sf::Keyboard::B,
		};

		this->_buttonsPressed = 0; // 0 = pressed
		for (int i = 0; i < 8; i++)
			if (!sf::Keyboard::isKeyPressed(buttons[i]))
				this->_buttonsPressed |= (1 << i);
	}

	int Joypad::getJoypadState(uint8_t joyp) const
	{
		bool checkDirections = !((joyp >> 4) & 0b1);
		bool checkActions = !((joyp >> 5) & 0b1);

		uint8_t newJoyp = (joyp & 0b00110000);
		if (checkDirections)
			newJoyp |= (this->_buttonsPressed & 0xF);
		if (checkActions)
			newJoyp |= (this->_buttonsPressed >> 4);

		return newJoyp;
	}
}
