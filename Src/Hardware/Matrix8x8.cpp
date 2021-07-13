//
// Created by koscum on 09/07/2021.
//

#include <bit>
#include <numeric>
#include "Matrix8x8.h"

Matrix8x8::Matrix8x8(uint16_t _address) :
		LedBackpack(_address) {}

void Matrix8x8::setBitmap(const std::array<uint8_t, 8> *_image)
{
	for (uint8_t i = 0; i < _image->size(); ++i) displayBuffer[i] = std::rotr((*_image)[i], 1);
}

void Matrix8x8::setPixel(const uint8_t _x, const uint8_t _y, const bool _value)
{
	if (_value) displayBuffer[_y % 8] |= std::rotr(0x1u << (_x % 8), 1);
	else displayBuffer[_y % 8] &= ~std::rotr(0x1u << (_x % 8), 1);
}

void Matrix8x8::togglePixel(const uint8_t _x, const uint8_t _y)
{
	displayBuffer[_y % 8] ^= std::rotr(0x1u << (_x % 8), 1);
}

void Matrix8x8::fill(const bool _value)
{
	for (uint8_t i = 0; i < 8; ++i) displayBuffer[i] = _value ? 0xff : 0x00;
}

void Matrix8x8::scroll(Direction _direction, uint8_t _value)
{
	switch (_direction)
	{
		case Direction::UP :
		case Direction::DOWN :
		{
			const uint8_t shift = _direction == Direction::UP ? _value % 8 : 8 - _value % 8;
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
			const uint8_t shift = _direction == Direction::LEFT ? _value % 8 : 8 - _value % 8;
			for (uint8_t i = 0; i < 8; ++i) displayBuffer[i] = std::rotr(displayBuffer[i], shift);
			break;
		}
	}
}
