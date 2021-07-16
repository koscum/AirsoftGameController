//
// Created by koscum on 01/07/2021.
//

#include "LedBackpack.h"
#include <stm32f4xx_hal.h>

LedBackpack::LedBackpack(const uint16_t _address) :
		I2cComponent(_address) {}

void LedBackpack::begin()
{
	auto data = new std::vector<uint8_t>{};
	data->push_back(LedBackpack::HT16K33_CMD_OSC | 0x01);
	transmit(data);

	clear();
	writeDisplay();

	setBlinkRate(BlinkRate::OFF);

	setBrightness(15);
}

void LedBackpack::setBrightness(uint8_t _brightness) const
{
	auto data = new std::vector<uint8_t>{};
	data->push_back(LedBackpack::HT16K33_CMD_BRIGHTNESS | std::min(_brightness, LedBackpack::MAX_BRIGHTNESS));
	transmit(data);
}

void LedBackpack::setBlinkRate(BlinkRate _blinkRate) const
{
	auto data = new std::vector<uint8_t>{};
	data->push_back(LedBackpack::HT16K33_CMD_BLINK | (static_cast<uint8_t>(_blinkRate) << 1) | 0x01);
	transmit(data);
}

void LedBackpack::clear()
{
	for (uint8_t &i : displayBuffer) i = 0x00;
}

void LedBackpack::writeDisplay() const
{
	auto data = new std::vector<uint8_t>{};
	for (auto displayValue : displayBuffer)
	{
		data->push_back(displayValue & 0xFF);
		data->push_back(displayValue >> 8);
	}
	writeRegister(HT16K33_ADR_DISPLAY, data);
}

constinit const std::array<uint8_t, 16> LedBackpack::NUMBER_TABLE{
		0x3F, /* 0 */
		0x06, /* 1 */
		0x5B, /* 2 */
		0x4F, /* 3 */
		0x66, /* 4 */
		0x6D, /* 5 */
		0x7D, /* 6 */
		0x07, /* 7 */
		0x7F, /* 8 */
		0x6F, /* 9 */
		0x77, /* A */
		0x7C, /* b */
		0x39, /* C */
		0x5E, /* d */
		0x79, /* E */
		0x71, /* F */
};

constexpr const uint8_t LedBackpack::HT16K33_ADR_DISPLAY = 0x00;
constexpr const uint8_t LedBackpack::HT16K33_CMD_OSC = 0x20;
constexpr const uint8_t LedBackpack::HT16K33_CMD_BRIGHTNESS = 0xE0;
constexpr const uint8_t LedBackpack::HT16K33_CMD_BLINK = 0x80;
constexpr const uint8_t LedBackpack::MAX_BRIGHTNESS = 15;
