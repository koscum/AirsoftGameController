//
// Created by koscum on 13/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_KEYPAD4X4_H
#define AIRSOFTGAMECONTROLLER_KEYPAD4X4_H


#include <cstdint>
#include "Mcp23008.h"

class Keypad4x4
{
public:
	enum class [[maybe_unused]] KeyState
	{
		RELEASED = 0,
		PRESSED,
	};

	enum class [[maybe_unused]] Key
	{
		ONE = 0,
		TWO,
		THREE,
		A,
		FOUR,
		FIVE,
		SIX,
		B,
		SEVEN,
		EIGHT,
		NINE,
		C,
		STAR,
		ZERO,
		HASH,
		D,
	};

	Keypad4x4() = delete;

	explicit Keypad4x4(uint16_t address);

	Keypad4x4(const Keypad4x4 &) = delete;

	Keypad4x4 &operator=(const Keypad4x4 &) = delete;

	void init();

	void tick();

	std::array<KeyState, 16> state;

private:
	Mcp23008 mcp23008;

	//std::array<KeyState, 16> state;
};


#endif //AIRSOFTGAMECONTROLLER_KEYPAD4X4_H
