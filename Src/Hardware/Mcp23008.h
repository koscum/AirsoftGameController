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
	enum class [[maybe_unused]] Mode
	{
		OUTPUT,
		INPUT,
	};

	enum class [[maybe_unused]] State
	{
		LOW,
		HIGH,
	};

	Mcp23008() = delete;

	explicit Mcp23008(uint16_t address);

	Mcp23008(const Mcp23008 &) = delete;

	Mcp23008 &operator=(const Mcp23008 &) = delete;

	void init();

	[[nodiscard]] uint8_t getGpio() const;

	[[nodiscard]] uint8_t getMode() const;

	void setGpio(uint8_t gpio,
	             const std::function<void()> *callback = nullptr);

	void setMode(uint8_t mode,
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
