//
// Created by koscum on 01/07/2021.
//

#include "LedBackpack.h"
#include "I2cController.h"

LedBackpack::LedBackpack(const uint16_t _address) :
		address(_address) {}

void LedBackpack::begin()
{
	auto data = std::vector<uint8_t>{};
	data.push_back(LedBackpack::HT16K33_OSC_CMD | 0x01);

	I2cController::getInstance()->send(address, &data);

	clear();
	writeDisplay();

	setBlinkRate(BlinkRate::OFF);

	setBrightness(15);
}

void LedBackpack::setBrightness(uint8_t _brightness) const
{
	auto data = std::vector<uint8_t>{};
	data.push_back(
			LedBackpack::HT16K33_CMD_BRIGHTNESS |
			_brightness <= LedBackpack::MAX_BRIGHTNESS ? LedBackpack::MAX_BRIGHTNESS : _brightness
	);

	I2cController::getInstance()->send(address, &data);
}

void LedBackpack::setBlinkRate(BlinkRate _blinkRate) const
{
	auto data = std::vector<uint8_t>{};
	data.push_back(LedBackpack::HT16K33_BLINK_CMD | (static_cast<uint8_t>(_blinkRate) << 1) | 0x01);

	I2cController::getInstance()->send(address, &data);
}

void LedBackpack::clear()
{
	for (uint8_t &i : displayBuffer) i = 0x00;
}

void LedBackpack::writeDisplay() const
{
	auto data = std::vector<uint8_t>{};

	data.push_back(0x00);
	for (auto displayValue : displayBuffer)
	{
		data.push_back(displayValue & 0xFF);
		data.push_back(displayValue >> 8);
	}

	I2cController::getInstance()->send(address, &data);
}

constexpr const uint8_t LedBackpack::NUMBER_TABLE[16] = {
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

constexpr const uint8_t LedBackpack::HT16K33_OSC_CMD = 0x20;
constexpr const uint8_t LedBackpack::HT16K33_CMD_BRIGHTNESS = 0xE0;
constexpr const uint8_t LedBackpack::HT16K33_BLINK_CMD = 0x80;
constexpr const uint8_t LedBackpack::MAX_BRIGHTNESS = 15;