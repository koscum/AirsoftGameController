//
// Created by koscum on 13/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_KEYPAD4X4_H
#define AIRSOFTGAMECONTROLLER_KEYPAD4X4_H


#include <cstdint>
#include <deque>
#include "Mcp23008.h"

class Keypad4x4
{
public:
	enum class KeyState
	{
		RELEASED = 0,
		PRESSED,
	};

	enum class Key
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

	explicit Keypad4x4(uint16_t address);

	void init();

	void tick();

	std::array<KeyState, 16> state;

private:
	const Mcp23008 mcp23008;

	//std::array<KeyState, 16> state;

	static const std::array<Mcp23008::Mode, 8> INPUT_ONLY;
	static const std::array<Mcp23008::Mode, 8> READ_COLUMNS;
	static const std::array<Mcp23008::Mode, 8> READ_ROWS;
	static const std::array<std::array<Mcp23008::Mode, 8>, 4> READ_ROW;
	static const std::array<Mcp23008::State, 8> OUTPUT_COLUMNS;
	static const std::array<std::array<Mcp23008::State, 8>, 4> OUTPUT_ROW;
};


#endif //AIRSOFTGAMECONTROLLER_KEYPAD4X4_H
