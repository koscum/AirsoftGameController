#ifndef AIRSOFTGAMECONTROLLER_SEVENSEGMENT_H
#define AIRSOFTGAMECONTROLLER_SEVENSEGMENT_H


#include "LedBackpack.h"

class SevenSegment : public LedBackpack
{
public:
	SevenSegment() = delete;

	explicit SevenSegment(uint16_t address);

	SevenSegment(const SevenSegment &) = delete;

	auto operator=(const SevenSegment &) -> SevenSegment & = delete;

	auto setDigit(uint8_t position, uint8_t value) -> void;

	auto setDot(uint8_t position, bool value) -> void;

	auto setColon(bool value) -> void;

	auto toggleDot(uint8_t position) -> void;

	auto toggleColon() -> void;

	auto clearDigit(uint8_t position) -> void;

	auto writeColon() const -> void;

private:
	[[nodiscard]] static auto digitPositionToDisplayBufferIndex(uint8_t _position) -> uint8_t;

	static const uint8_t COLON_ADR;
	static const uint8_t COLON_BUFFER_POSITION;
	static const uint8_t COLON_ON;
	static const uint8_t DOT_ON;
};


#endif //AIRSOFTGAMECONTROLLER_SEVENSEGMENT_H
