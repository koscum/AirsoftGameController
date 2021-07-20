//
// Created by koscum on 13/07/2021.
//

#include <stm32f4xx_hal.h>
#include "Keypad4x4.h"

Keypad4x4::Keypad4x4(uint16_t address) :
		mcp23008(Mcp23008(address)),
		state(std::array<KeyState, 16>{KeyState::RELEASED}) {}

void Keypad4x4::init()
{
	mcp23008.init();
	mcp23008.setMode(0x0f);
}

void Keypad4x4::tick()
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		mcp23008.setGpio((~(0x10 << i) & 0xf0) | (mcp23008.getGpio() & 0x0f));
		mcp23008.readGpio(new std::function<void()>(
				[&, i]()
				{
					auto gpio = mcp23008.getGpio();
					for (uint8_t j = 0; j < 4; ++j)
					{
						state[i * 4 + j] = gpio & (0x1 << j) ?
						                   KeyState::RELEASED :
						                   KeyState::PRESSED;
					}
				}
		));
	}
}
