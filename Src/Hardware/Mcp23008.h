//
// Created by koscum on 12/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_MCP23008_H
#define AIRSOFTGAMECONTROLLER_MCP23008_H


#include <array>
#include <cstdint>
#include "../I2c/I2cComponent.h"

class Mcp23008 : public I2cComponent
{
public:
	enum class Mode
	{
		OUTPUT,
		INPUT,
	};

	enum class State
	{
		LOW,
		HIGH,
	};

	explicit Mcp23008(uint16_t address);

	void init() const;

	void setMode(const std::array<Mode, 8> *mode,
	             const std::function<void()> *callback = nullptr) const;

	void setLatch(const std::array<State, 8> *state,
	              const std::function<void()> *callback = nullptr) const;

	void readGpio(const std::function<void(std::array<State, 8> *)> *callback = nullptr) const;

private:
	static const uint8_t MCP23008_ADR_IODIR;
	static const uint8_t MCP23008_ADR_IPOL;
	static const uint8_t MCP23008_ADR_GPINTEN;
	static const uint8_t MCP23008_ADR_DEFVAL;
	static const uint8_t MCP23008_ADR_INTCON;
	static const uint8_t MCP23008_ADR_IOCON;
	static const uint8_t MCP23008_ADR_GPPU;
	static const uint8_t MCP23008_ADR_INTF;
	static const uint8_t MCP23008_ADR_INTCAP;
	static const uint8_t MCP23008_ADR_GPIO;
	static const uint8_t MCP23008_ADR_OLAT;
	static const uint8_t MCP23008_IOCON_SEQOP;
	static const uint8_t MCP23008_MASK_GPIO_LOW;
	static const uint8_t MCP23008_MASK_GPIO_HIGH;
};


#endif //AIRSOFTGAMECONTROLLER_MCP23008_H
