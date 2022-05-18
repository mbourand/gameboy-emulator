#pragma once

#include <cstdint>

namespace gbmu
{
	class Controller
	{
	protected:
		uint8_t _buttonsPressed;

	public:
		Controller() = default;
		Controller(const Controller& other) = default;
		Controller& operator=(const Controller& other) = default;

		void update();
		int getJoypadState(uint8_t joyp) const;
		void handleInterrupt(uint8_t value);
	};
}
