#include "PPU.hpp"
#include <iostream>

namespace gbmu
{
	PPU::PPU(Memory& memory) : _memory(memory), _state(State::OAMSearch), _ticks(0)
	{
		this->_lcdPixels.create(160, 144);
		this->_finishedLcdPixels.create(160, 144);

		this->_gbColors[0] = sf::Color::White;
		this->_gbColors[1] = sf::Color::Color(255 * 2 / 3, 255 * 2 / 3, 255 * 2 / 3);
		this->_gbColors[2] = sf::Color::Color(255 / 3, 255 / 3, 255 / 3);
		this->_gbColors[3] = sf::Color::Black;
	}

	void PPU::tick()
	{
		uint8_t lcdc = this->_memory.readByte(0xFF40);
		if ((lcdc & LCDC::LCDEnable) == 0)
		{
			this->_state = State::OAMSearch;
			this->_ticks = 0;
			this->_memory.writeByte(0xFF44, 0);
			this->_updateLCDStatus(0x0);
			return;
		}
		auto entryState = this->_state;
		switch (this->_state)
		{
			case State::OAMSearch:
				if (this->_ticks == 20)
				{
					this->_state = State::PixelTransfer;
					this->_updateLCDStatus(0x0);
				}
				break;
			case State::PixelTransfer:
				if (this->_ticks == 43)
				{
					this->_pixelTransfer();
					this->_state = State::HBlank;
					this->_updateLCDStatus(LCDStatus::HBlankInt);
				}
				break;
			case State::HBlank:
				if (this->_ticks == 51)
				{
					this->_memory.writeByte(0xFF44, this->_memory.readByte(0xFF44) + 1);
					if (this->_memory.readByte(0xFF44) == 144)
					{
						this->_state = State::VBlank;
						this->_updateLCDStatus(LCDStatus::VBlankInt | LCDStatus::LycLyCoincidenceInt);
						this->_finishedLcdPixels = this->_lcdPixels;
					}
					else
					{
						this->_state = State::OAMSearch;
						this->_updateLCDStatus(LCDStatus::OAMSearchInt | LCDStatus::LycLyCoincidenceInt);
					}
				}
				break;
			case State::VBlank:
				if (this->_ticks % (20 + 43 + 51) == 0 && this->_ticks != 0)
				{
					this->_memory.writeByte(0xFF44, this->_memory.readByte(0xFF44) + 1);
					this->_updateLCDStatus(LCDStatus::LycLyCoincidenceInt);
				}
				if (this->_ticks == (20 + 43 + 51) * 10)
				{
					this->_memory.writeByte(0xFF44, 0);
					this->_state = State::OAMSearch;
					this->_updateLCDStatus(LCDStatus::OAMSearchInt);
				}
				break;
		}

		this->_updateLCDStatus(0x0);
		if (entryState != this->_state)
			this->_ticks = 0;
		else
			this->_ticks++;
	}

	void PPU::_updateLCDStatus(uint8_t interruptsToCheck)
	{
		LCDStatus mode;
		/* clang-format off */
		switch (this->_state)
		{
			case State::OAMSearch: mode = LCDStatus::OAMSearchMode; break;
			case State::PixelTransfer: mode = LCDStatus::PixelTransferMode; break;
			case State::HBlank: mode = LCDStatus::HBlankMode; break;
			case State::VBlank: mode = LCDStatus::VBlankMode; break;
		}
		/* clang-format on */
		uint8_t lcdStatus = this->_memory.readByte(0xFF41);
		uint8_t ly = this->_memory.readByte(0xFF44);
		uint8_t lyc = this->_memory.readByte(0xFF45);
		lcdStatus = (lcdStatus & 0b11111000) | mode | ((ly == lyc) << 2);
		this->_memory.writeByte(0xFF41, lcdStatus);
		if (ly == lyc && (lcdStatus & interruptsToCheck & LCDStatus::LycLyCoincidenceInt))
			this->_memory.writeByte(0xFF0F, this->_memory.readByte(0xFF0F) | Interrupt::LCDStat);
		if (mode == LCDStatus::VBlankMode && (lcdStatus & interruptsToCheck & LCDStatus::VBlankInt))
			this->_memory.writeByte(0xFF0F, this->_memory.readByte(0xFF0F) | Interrupt::LCDStat);
		if (mode == LCDStatus::HBlankMode && (lcdStatus & interruptsToCheck & LCDStatus::HBlankInt))
			this->_memory.writeByte(0xFF0F, this->_memory.readByte(0xFF0F) | Interrupt::LCDStat);
		if (mode == LCDStatus::OAMSearchMode && (lcdStatus & interruptsToCheck & LCDStatus::OAMSearchInt))
			this->_memory.writeByte(0xFF0F, this->_memory.readByte(0xFF0F) | Interrupt::LCDStat);
	}

	void PPU::_pixelTransfer()
	{
		uint8_t ly = this->_memory.readByte(0xFF44);
		if (ly < 0 || ly > 143)
			return;
		uint8_t scy = this->_memory.readByte(0xFF42);
		uint8_t scx = this->_memory.readByte(0xFF43);

		uint8_t bgPalette = this->_memory.readByte(0xFF47);

		uint8_t lcdc = this->_memory.readByte(0xFF40);

		uint16_t bgTileMap = (lcdc & LCDC::BgTileMapArea) ? 0x9800 : 0x9C00;
		uint16_t tileData = (lcdc & LCDC::BgWinTileDataArea) ? 0x8000 : 0x9000;

		uint8_t yPos = scy + ly;
		uint8_t tileRowInMap = yPos / 8;

		for (uint8_t x = 0; x < 160; x++)
		{
			uint8_t xPos = scx + x;
			uint8_t tileColInMap = xPos / 8;

			uint16_t tileAddressInMap = bgTileMap + tileRowInMap * 32 + tileColInMap;
			uint16_t tileOffsetInData = 0;
			if (lcdc & LCDC::BgTileMapArea)
				tileOffsetInData = static_cast<int8_t>(this->_memory.readByte(tileAddressInMap));
			else
				tileOffsetInData = this->_memory.readByte(tileAddressInMap);

			uint16_t tileAddressInData = tileData;
			if (lcdc & LCDC::BgWinTileDataArea)
				tileAddressInData += tileOffsetInData * 16;
			else
				tileAddressInData += (tileOffsetInData + 128) * 16;

			uint8_t tileRow = (yPos % 8);
			uint8_t tileCol = (xPos % 8);

			uint8_t tileDataLow = this->_memory.readByte(tileAddressInData + tileRow * 2);
			uint8_t tileDataHigh = this->_memory.readByte(tileAddressInData + tileRow * 2 + 1);

			uint8_t colorId = (((tileDataLow >> (7 - tileCol)) & 1) << 1) | ((tileDataHigh >> (7 - tileCol)) & 1);

			sf::Color color = this->getPaletteColor(colorId);
			this->_lcdPixels.setPixel(x, ly, color);
		}
	}

	sf::Color PPU::getPaletteColor(uint8_t index) const
	{
		uint8_t paletteColor = this->_memory.readByte(0xFF47);
		uint8_t colorId = (paletteColor >> (2 * index)) & 0b11;
		return this->_gbColors[colorId];
	}

	const sf::Image& PPU::getImage() const { return this->_finishedLcdPixels; }
}
