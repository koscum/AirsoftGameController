//
// Created by koscum on 12/07/2021.
//

#include "Mcp23008.h"

Mcp23008::Mcp23008(uint16_t address) :
		I2cComponent(address) {}

void Mcp23008::init() const
{
	auto data = std::vector<uint8_t>{};
	data.push_back(MCP23008_IOCON_SEQOP);
	writeRegister(MCP23008_ADR_IOCON, &data);

	data.clear();
	data.push_back(0xf0);
	writeRegister(MCP23008_ADR_IODIR, &data);

	data.clear();
	data.push_back(0xff);
	writeRegister(MCP23008_ADR_IPOL, &data);

	data.clear();
	data.push_back(0xff);
	writeRegister(MCP23008_ADR_GPPU, &data);

	data.clear();
	data.push_back(0x00);
	writeRegister(MCP23008_ADR_OLAT, &data);
}

bool Mcp23008::setMode(const std::array<Mcp23008::Mode, 8> *_mode) const
{
	uint8_t mode = 0x00;
	for (uint8_t i = 0; i < 8; ++i) mode |= static_cast<uint8_t>((*_mode)[i]) << i;

	auto data = std::vector<uint8_t>{};
	data.push_back(mode);

	return writeRegister(MCP23008_ADR_IODIR, &data);
}

bool Mcp23008::setLatch(const std::array<Mcp23008::State, 8> *_state) const
{
	uint8_t state = 0x00;
	for (uint8_t i = 0; i < 8; ++i) state |= static_cast<uint8_t>((*_state)[i]) << i;

	auto data = std::vector<uint8_t>{};
	data.push_back(state);

	return writeRegister(MCP23008_ADR_OLAT, &data);
}

std::array<Mcp23008::State, 8> *Mcp23008::readGpio() const
{
	auto data = new std::array<State, 8>();

	auto gpioData = readRegister(MCP23008_ADR_GPIO, 1);
	for (int i = 0; i < data->size(); i++) (*data)[i] = (*gpioData)[0] & (0x1 << i) ? State::HIGH : State::LOW;
	delete gpioData;

	return data;
}

constexpr uint8_t Mcp23008::MCP23008_ADR_IODIR = 0x00;
constexpr uint8_t Mcp23008::MCP23008_ADR_IPOL = 0x01;
constexpr uint8_t Mcp23008::MCP23008_ADR_GPINTEN = 0x02;
constexpr uint8_t Mcp23008::MCP23008_ADR_DEFVAL = 0x03;
constexpr uint8_t Mcp23008::MCP23008_ADR_INTCON = 0x04;
constexpr uint8_t Mcp23008::MCP23008_ADR_IOCON = 0x05;
constexpr uint8_t Mcp23008::MCP23008_ADR_GPPU = 0x06;
constexpr uint8_t Mcp23008::MCP23008_ADR_INTF = 0x07;
constexpr uint8_t Mcp23008::MCP23008_ADR_INTCAP = 0x08;
constexpr uint8_t Mcp23008::MCP23008_ADR_GPIO = 0x09;
constexpr uint8_t Mcp23008::MCP23008_ADR_OLAT = 0x0a;
constexpr uint8_t Mcp23008::MCP23008_IOCON_SEQOP = 0x20;
