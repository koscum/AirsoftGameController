//
// Created by koscum on 01/07/2021.
//

#include "SevenSegment.h"
#include "I2cController.h"
#include <vector>

SevenSegment::SevenSegment(uint16_t address) :
		LedBackpack(address) {}

void SevenSegment::setDigit(uint8_t _position, uint8_t _value)
{
	displayBuffer[_position > 1 ? _position > 3 ? 4 : _position + 1 : _position] =
			LedBackpack::NUMBER_TABLE[_value > 15 ? 15 : _value];
}

void SevenSegment::setDot(uint8_t _position, bool _value)
{
	displayBuffer[_position > 1 ? _position > 3 ? 4 : _position + 1 : _position] |=
			_value ? DOT_ON : 0x0;
}

void SevenSegment::setColon(bool _value)
{
	displayBuffer[SevenSegment::COLON_BUFFER_POSITION] = _value ? SevenSegment::COLON_ON : 0x0;
}

void SevenSegment::toggleDot(uint8_t _position)
{
	displayBuffer[_position > 1 ? _position > 3 ? 4 : _position + 1 : _position] ^= SevenSegment::DOT_ON;
}

void SevenSegment::toggleColon()
{
	displayBuffer[SevenSegment::COLON_BUFFER_POSITION] ^= SevenSegment::COLON_ON;
}

void SevenSegment::clearDigit(uint8_t _position)
{
	displayBuffer[_position > 1 ? _position > 3 ? 4 : _position + 1 : _position] &= DOT_ON;
}

void SevenSegment::writeColon() const
{
	auto data = std::vector<uint8_t>{};

	data.push_back(COLON_ADDR);
	data.push_back(displayBuffer[SevenSegment::COLON_BUFFER_POSITION] & 0xFF);
	data.push_back(displayBuffer[SevenSegment::COLON_BUFFER_POSITION] >> 8);

	I2cController::getInstance()->send(address, &data);
}

constexpr const uint8_t SevenSegment::COLON_ADDR = 0x04;
constexpr const uint8_t SevenSegment::COLON_BUFFER_POSITION = 2;
constexpr const uint8_t SevenSegment::COLON_ON = 0x2;
constexpr const uint8_t SevenSegment::DOT_ON = 0x80;
