#ifndef AIRSOFTGAMECONTROLLER_MATRIX8X8_H
#define AIRSOFTGAMECONTROLLER_MATRIX8X8_H


#include <array>
#include "LedBackpack.h"

class Matrix8x8 : public LedBackpack
{
public:
	enum class [[maybe_unused]] Direction
	{
		UP,
		LEFT,
		DOWN,
		RIGHT,
	};

	Matrix8x8() = delete;

	explicit Matrix8x8(uint16_t address);

	Matrix8x8(const Matrix8x8 &) = delete;

	auto operator=(const Matrix8x8 &) -> Matrix8x8 & = delete;

	auto setBitmap(const std::array<uint8_t, 8> *image) -> void;

	auto setPixel(uint8_t x, uint8_t y, bool value) -> void;

	auto togglePixel(uint8_t x, uint8_t y) -> void;

	auto fill(bool value = true) -> void;

	auto scroll(Direction direction, uint8_t value) -> void;
};


#endif //AIRSOFTGAMECONTROLLER_MATRIX8X8_H
