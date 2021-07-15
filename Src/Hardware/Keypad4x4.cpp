//
// Created by koscum on 13/07/2021.
//

#include <vector>
#include "Keypad4x4.h"

Keypad4x4::Keypad4x4(uint16_t address) :
		mcp23008(Mcp23008(address)),
		state(std::array<KeyState, 16>{
				KeyState::RELEASED,
		}) {}

void Keypad4x4::init()
{
	mcp23008.init();
	mcp23008.setMode(&INPUT_ONLY);
}

void Keypad4x4::tick()
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		mcp23008.setMode(&READ_ROW[i]);
		auto gpioRows = mcp23008.readGpio();

		for (uint8_t j = 0; j < 4; ++j)
		{
			state[i * 4 + j] = (*gpioRows)[j] == Mcp23008::State::HIGH ? KeyState::PRESSED : KeyState::RELEASED;
		}

		delete gpioRows;
	}

	mcp23008.setMode(&INPUT_ONLY);
}

constinit const std::array<Mcp23008::Mode, 8> Keypad4x4::INPUT_ONLY{
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
		Mcp23008::Mode::INPUT,
};

constinit const std::array<std::array<Mcp23008::Mode, 8>, 4> Keypad4x4::READ_ROW{
		std::array<Mcp23008::Mode, 8>{
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::OUTPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
		},
		std::array<Mcp23008::Mode, 8>{
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::OUTPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
		},
		std::array<Mcp23008::Mode, 8>{
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::OUTPUT,
				Mcp23008::Mode::INPUT,
		},
		std::array<Mcp23008::Mode, 8>{
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::INPUT,
				Mcp23008::Mode::OUTPUT,
		},
};
