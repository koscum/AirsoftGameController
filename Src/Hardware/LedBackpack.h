#ifndef AIRSOFTGAMECONTROLLER_LEDBACKPACK_H
#define AIRSOFTGAMECONTROLLER_LEDBACKPACK_H


#include <array>
#include <cstdint>
#include "../I2c/I2cComponent.h"

class LedBackpack : public I2cComponent
{
public:
	enum class [[maybe_unused]] BlinkRate
	{
		OFF,
		TWO_HZ,
		ONE_HZ,
		HALF_HZ,
	};

	LedBackpack() = delete;

	explicit LedBackpack(uint16_t address);

	LedBackpack(const LedBackpack &) = delete;

	auto operator=(const LedBackpack &) -> LedBackpack & = delete;

	auto begin() -> void;

	auto setBrightness(uint8_t brightness) const -> void;

	auto setBlinkRate(BlinkRate blinkRate) const -> void;

	auto clear() -> void;

	auto writeDisplay() -> void;

protected:
	std::array<uint8_t, 8> displayBuffer;

	static const std::array<uint8_t, 16> NUMBER_TABLE;

private:
	static const uint8_t HT16K33_ADR_DISPLAY;
	static const uint8_t HT16K33_CMD_BLINK;
	static const uint8_t HT16K33_CMD_BRIGHTNESS;
	static const uint8_t HT16K33_CMD_OSC;
	static const uint8_t MAX_BRIGHTNESS;
};


#endif //AIRSOFTGAMECONTROLLER_LEDBACKPACK_H
