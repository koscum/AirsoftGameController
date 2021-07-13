//
// Created by koscum on 09/07/2021.
//

#include <bit>
#include <numeric>
#include "Matrix8x8.h"

Matrix8x8::Matrix8x8(uint16_t address) :
		LedBackpack(address) {}

void Matrix8x8::setBitmap(std::array<uint8_t, 8> image)
{
	for (uint8_t i = 0; i < image.size(); ++i) displayBuffer[i] = std::rotr((unsigned char) image[i], 1);
}

void Matrix8x8::setPixel(const uint8_t x, const uint8_t y, const bool value)
{
	if (value) displayBuffer[y % 8] |= std::rotr((unsigned char) (0x1 << x % 8), 1);
	else displayBuffer[y % 8] &= ~std::rotr((unsigned char) (0x1 << x % 8), 1);
}

void Matrix8x8::togglePixel(const uint8_t x, const uint8_t y)
{
	displayBuffer[y % 8] ^= std::rotr((unsigned char) (0x1 << x % 8), 1);
}

void Matrix8x8::fill(const bool value)
{
	for (uint8_t i = 0; i < 8; ++i) displayBuffer[i] = value ? 0xff : 0x00;
}

void Matrix8x8::scroll(Direction direction, uint8_t value)
{
	switch (direction)
	{
		case Direction::UP :
		case Direction::DOWN :
		{
			const uint8_t shift = (direction == Direction::UP ? value : 8 - value) % 8;
			const int8_t gcd = std::gcd(shift, 8);

			for (uint8_t i = 0; i < gcd; i++)
			{
				uint8_t temp = displayBuffer[i];
				uint8_t j = i;

				while (true)
				{
					uint8_t k = (j + shift) % 8;

					if (k == i) break;

					displayBuffer[j] = displayBuffer[k];
					j = k;
				}
				displayBuffer[j] = temp;
			}

			break;
		}
		case Direction::LEFT :
		case Direction::RIGHT :
		{
			const uint8_t shift = (direction == Direction::LEFT ? value : 8 - value) % 8;
			for (uint8_t i = 0; i < 8; ++i) displayBuffer[i] = std::rotr(displayBuffer[i], shift);
			break;
		}
	}
}
