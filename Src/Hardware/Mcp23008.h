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

	void init();

	uint8_t getGpio() const;

	uint8_t getMode() const;

	void setGpio(const uint8_t gpio,
	             const std::function<void()> *callback = nullptr);

	void setMode(const uint8_t mode,
	             const std::function<void()> *callback = nullptr);

	void readGpio(const std::function<void()> *callback = nullptr);

	void readMode(const std::function<void()> *callback = nullptr);

	static const uint8_t MCP23008_GPIO_MODE_INPUT;
	static const uint8_t MCP23008_GPIO_MODE_OUTPUT;

private:
	uint8_t gpio;
	uint8_t mode;

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
