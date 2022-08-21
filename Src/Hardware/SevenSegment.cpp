#include <vector>
#include "SevenSegment.h"

SevenSegment::SevenSegment(uint16_t _address) :
		LedBackpack(_address) {}

auto SevenSegment::setDigit(uint8_t _position, uint8_t _value) -> void
{
	displayBuffer[digitPositionToDisplayBufferIndex(_position)] =
			(displayBuffer[digitPositionToDisplayBufferIndex(_position)] & DOT_ON) |
			NUMBER_TABLE[std::min(_value, static_cast<uint8_t>(NUMBER_TABLE.size() - 1))];
}

auto SevenSegment::setDot(uint8_t _position, bool _value) -> void
{
	if (_value) displayBuffer[digitPositionToDisplayBufferIndex(_position)] |= DOT_ON;
	else displayBuffer[digitPositionToDisplayBufferIndex(_position)] &= ~DOT_ON;
}

auto SevenSegment::setColon(bool _value) -> void
{
	displayBuffer[SevenSegment::COLON_BUFFER_POSITION] = _value ? SevenSegment::COLON_ON : 0x0;
}

auto SevenSegment::toggleDot(uint8_t _position) -> void
{
	displayBuffer[digitPositionToDisplayBufferIndex(_position)] ^= SevenSegment::DOT_ON;
}

auto SevenSegment::toggleColon() -> void
{
	displayBuffer[SevenSegment::COLON_BUFFER_POSITION] ^= SevenSegment::COLON_ON;
}

auto SevenSegment::clearDigit(uint8_t _position) -> void
{
	displayBuffer[digitPositionToDisplayBufferIndex(_position)] &= DOT_ON;
}

auto SevenSegment::writeColon() const -> void
{
	auto data = std::vector<uint8_t>{};
	data.push_back(displayBuffer[SevenSegment::COLON_BUFFER_POSITION] & 0xFF);
	data.push_back(displayBuffer[SevenSegment::COLON_BUFFER_POSITION] >> 8);
	writeRegister(COLON_ADR, &data);
}

auto SevenSegment::digitPositionToDisplayBufferIndex(uint8_t _position) -> uint8_t
{
	return _position > 1 ? (_position > 3 ? 4 : _position + 1) : _position;
}

constinit const uint8_t SevenSegment::COLON_ADR = 0x04;
constinit const uint8_t SevenSegment::COLON_BUFFER_POSITION = 2;
constinit const uint8_t SevenSegment::COLON_ON = 0x02;
constinit const uint8_t SevenSegment::DOT_ON = 0x80;
