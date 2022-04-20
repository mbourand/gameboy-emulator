#include "Cartridge.hpp"
#include <fstream>
#include <stdexcept>

namespace gbmu
{
	Cartridge::Cartridge(const std::string& filename) { this->loadROM(filename); }

	void Cartridge::loadROM(const std::string& filename)
	{
		this->_bytes.clear();

		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			throw std::runtime_error("Could not open file: " + filename);

		const auto size = file.tellg();
		file.seekg(0, std::ios::beg);

		_bytes.resize(size);
		file.read(reinterpret_cast<char*>(_bytes.data()), size);
	}
}
