//
// Created by koscum on 01/07/2021.
//

#include <algorithm>
#include <vector>
#include "SevenSegment.h"

SevenSegment::SevenSegment(uint16_t _address) :
		LedBackpack(_address) {}

void SevenSegment::setDigit(uint8_t _position, uint8_t _value)
{
	displayBuffer[digitPositionToDisplayBufferIndex(_position)] =
			(displayBuffer[digitPositionToDisplayBufferIndex(_position)] & DOT_ON) |
			NUMBER_TABLE[std::min(_value, static_cast<uint8_t>(NUMBER_TABLE.size() - 1))];
}

void SevenSegment::setDot(uint8_t _position, bool _value)
{
	if (_value) displayBuffer[digitPositionToDisplayBufferIndex(_position)] |= DOT_ON;
	else displayBuffer[digitPositionToDisplayBufferIndex(_position)] &= ~DOT_ON;
}

void SevenSegment::setColon(bool _value)
{
	displayBuffer[SevenSegment::COLON_BUFFER_POSITION] = _value ? SevenSegment::COLON_ON : 0x0;
}

void SevenSegment::toggleDot(uint8_t _position)
{
	displayBuffer[digitPositionToDisplayBufferIndex(_position)] ^= SevenSegment::DOT_ON;
}

void SevenSegment::toggleColon()
{
	displayBuffer[SevenSegment::COLON_BUFFER_POSITION] ^= SevenSegment::COLON_ON;
}

void SevenSegment::clearDigit(uint8_t _position)
{
	displayBuffer[digitPositionToDisplayBufferIndex(_position)] &= DOT_ON;
}

void SevenSegment::writeColon() const
{
	auto data = std::vector<uint8_t>{};
	data.push_back(displayBuffer[SevenSegment::COLON_BUFFER_POSITION] & 0xFF);
	data.push_back(displayBuffer[SevenSegment::COLON_BUFFER_POSITION] >> 8);
	writeRegister(COLON_ADR, &data);
}

uint8_t SevenSegment::digitPositionToDisplayBufferIndex(uint8_t _position)
{
	return _position > 1 ? (_position > 3 ? 4 : _position + 1) : _position;
}

constinit const uint8_t SevenSegment::COLON_ADR = 0x04;
constinit const uint8_t SevenSegment::COLON_BUFFER_POSITION = 2;
constinit const uint8_t SevenSegment::COLON_ON = 0x02;
constinit const uint8_t SevenSegment::DOT_ON = 0x80;
