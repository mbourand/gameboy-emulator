#pragma once

#include "Memory.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
#include <array>

namespace gbmu
{
	class Gameboy;

	enum OAMFlags
	{
		BgWinOverOBJ = 0b10000000,
		FlipY = 0b01000000,
		FlipX = 0b00100000,
		PaletteNumber = 0b00010000,
	};

	class OAMEntry
	{
	public:
		OAMEntry() = default;
		OAMEntry(const OAMEntry& other) = default;
		OAMEntry& operator=(const OAMEntry& other) = default;
		OAMEntry(uint8_t x, uint8_t y, uint8_t tileIndex, uint8_t flags);

		uint8_t x;
		uint8_t y;
		uint8_t tileIndex;
		uint8_t flags;
	};

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
		Gameboy& _gb;
		State _state;
		uint32_t _ticks;
		std::array<sf::Color, 4> _gbColors;
		std::vector<OAMEntry> _oamEntries;

		sf::Image _lcdPixels, _finishedLcdPixels, _blankLcdPixels;

		bool _lcdWasDisabled;

		uint8_t _winLineCounter;
		std::vector<OAMEntry> _selectedOamEntries;

	public:
		PPU() = default;
		PPU(Gameboy& memory);
		PPU& operator=(const PPU&) = default;

		void tick();

		const sf::Image& getImage() const;

		sf::Color getPaletteColor(uint8_t index, uint8_t palette, bool transparency) const;

	protected:
		void _oamSearch();
		void _pixelTransfer();
		void _updateLCDStatus(uint8_t interruptsToCheck);
	};
}
