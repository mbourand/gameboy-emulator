#pragma once

#include "Memory.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
#include <array>

namespace gbmu
{
	class PPU
	{
	public:
		static constexpr double PPU_FREQUENCY = 1000000000.0 / (4194304.0 / 4.0);

		enum State : uint8_t
		{
			OAMSearch = 0b00000001,
			PixelTransfer = 0b00000010,
			HBlank = 0b00000100,
			VBlank = 0b00001000,
		};

		enum LCDC : uint8_t
		{
			BgWinEnable = 0b00000001,
			ObjEnable = 0b00000010,
			ObjSize = 0b00000100,
			BgTileMapArea = 0b00001000,
			BgWinTileDataArea = 0b00010000,
			WinEnable = 0b00100000,
			WinTileMapArea = 0b01000000,
			LCDEnable = 0b10000000
		};

		enum LCDStatus : uint8_t
		{
			HBlankMode = 0,
			VBlankMode = 1,
			OAMSearchMode = 2,
			PixelTransferMode = 3,
			LycLyCoincidenceInt = 0b01000000,
			OAMSearchInt = 0b00100000,
			VBlankInt = 0b00010000,
			HBlankInt = 0b00001000
		};

	protected:
		Memory& _memory;
		State _state;
		uint32_t _ticks;
		std::array<sf::Color, 4> _gbColors;

		sf::Image _lcdPixels, _finishedLcdPixels;

	public:
		PPU(Memory& memory);

		void tick();

		const sf::Image& getImage() const;

		sf::Color getPaletteColor(uint8_t index) const;

	protected:
		void _pixelTransfer();
		void _updateLCDStatus(uint8_t interruptsToCheck);
	};
}
