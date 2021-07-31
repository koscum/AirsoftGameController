//
// Created by koscum on 09/07/2021.
//

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

	Matrix8x8 &operator=(const Matrix8x8 &) = delete;

	void setBitmap(const std::array<uint8_t, 8> *image);

	void setPixel(uint8_t x, uint8_t y, bool value);

	void togglePixel(uint8_t x, uint8_t y);

	void fill(bool value = true);

	void scroll(Direction direction, uint8_t value);
};


#endif //AIRSOFTGAMECONTROLLER_MATRIX8X8_H
