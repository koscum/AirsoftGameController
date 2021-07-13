//
// Created by koscum on 01/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_LEDBACKPACK_H
#define AIRSOFTGAMECONTROLLER_LEDBACKPACK_H


#include <array>
#include <cstdint>
#include "../I2c/I2cComponent.h"

class LedBackpack : public I2cComponent
{
public:
	enum class BlinkRate
	{
		OFF = 0,
		TWO_HZ = 1,
		ONE_HZ = 2,
		HALF_HZ = 3,
	};

	explicit LedBackpack(uint16_t address);

	void begin();

	void setBrightness(uint8_t brightness) const;

	void setBlinkRate(BlinkRate blinkRate) const;

	void clear();

	void writeDisplay() const;

protected:
	std::array<uint8_t, 8> displayBuffer{};

	static const std::array<uint8_t, 16> NUMBER_TABLE;

private:
	static const uint8_t HT16K33_ADR_DISPLAY;
	static const uint8_t HT16K33_CMD_BLINK;
	static const uint8_t HT16K33_CMD_BRIGHTNESS;
	static const uint8_t HT16K33_CMD_OSC;
	static const uint8_t MAX_BRIGHTNESS;
};


#endif //AIRSOFTGAMECONTROLLER_LEDBACKPACK_H
