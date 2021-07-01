//
// Created by koscum on 01/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_LEDBACKPACK_H
#define AIRSOFTGAMECONTROLLER_LEDBACKPACK_H


#include <cstdint>
#include <array>

class LedBackpack
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
	const uint16_t address;
	std::array<uint8_t, 8> displayBuffer{};

	static const std::array<uint8_t, 16> NUMBER_TABLE;

private:
	static const uint8_t HT16K33_BLINK_CMD;
	static const uint8_t HT16K33_CMD_BRIGHTNESS;
	static const uint8_t HT16K33_OSC_CMD;
	static const uint8_t MAX_BRIGHTNESS;
};


#endif //AIRSOFTGAMECONTROLLER_LEDBACKPACK_H
