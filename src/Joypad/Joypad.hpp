#pragma once

#include <cstdint>

namespace gbmu
{
	class Joypad
	{
	protected:
		uint8_t _buttonsPressed;

	public:
		Joypad() = default;
		Joypad(const Joypad& other) = default;
		Joypad& operator=(const Joypad& other) = default;

		void update();
		int getJoypadState(uint8_t joyp) const;
	};
}
