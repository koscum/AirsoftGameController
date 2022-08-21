#include "Mcp23008.h"

Mcp23008::Mcp23008(uint16_t address) :
		I2cComponent(address),
		mode(MCP23008_GPIO_MODE_INPUT),
		gpio(0x00) {}

auto Mcp23008::init() -> void
{
	auto dataIocon = new std::vector<uint8_t>{};
	dataIocon->push_back(MCP23008_IOCON_SEQOP);
	writeRegister(MCP23008_ADR_IOCON,
	              dataIocon);

	auto dataIpol = new std::vector<uint8_t>{};
	dataIpol->push_back(0x00);
	writeRegister(MCP23008_ADR_IPOL,
	              dataIpol);

	auto dataGppu = new std::vector<uint8_t>{};
	dataGppu->push_back(0xff);
	writeRegister(MCP23008_ADR_GPPU,
	              dataGppu);

	auto dataOlat = new std::vector<uint8_t>{};
	dataOlat->push_back(gpio);
	writeRegister(MCP23008_ADR_OLAT,
	              dataOlat);

	setMode(mode);
	setGpio(gpio);
}

auto Mcp23008::getGpio() const -> uint8_t
{
	return gpio;
}

auto Mcp23008::getMode() const -> uint8_t
{
	return mode;
}

auto Mcp23008::setGpio(const uint8_t _gpio,
                       const std::function<void()> *_callback) -> void
{
	gpio = _gpio;

	auto data = new std::vector<uint8_t>{};
	data->push_back(gpio);

	writeRegister(MCP23008_ADR_GPIO,
	              data,
	              _callback);
}

auto Mcp23008::setMode(const uint8_t _mode,
                       const std::function<void()> *_callback) -> void
{
	mode = _mode;

	auto data = new std::vector<uint8_t>{};
	data->push_back(mode);

	writeRegister(MCP23008_ADR_IODIR,
	              data,
	              _callback);
}

auto Mcp23008::readGpio(const std::function<void()> *_callback) -> void
{
	readRegister(MCP23008_ADR_GPIO,
	             1,
	             new std::function<void(std::vector<uint8_t> *)>(
			             [&, _callback](std::vector<uint8_t> *gpioData)
			             {
				             gpio = (*gpioData)[0];
				             if (_callback != nullptr) (*_callback)();
				             delete _callback;
			             }
	             ));
}

auto Mcp23008::readMode(const std::function<void()> *_callback) -> void
{
	readRegister(MCP23008_ADR_IODIR,
	             1,
	             new std::function<void(std::vector<uint8_t> *)>(
			             [&, _callback](std::vector<uint8_t> *modeData)
			             {
				             mode = (*modeData)[0];
				             if (_callback != nullptr) (*_callback)();
				             delete _callback;
			             }
	             ));
}

constinit const uint8_t Mcp23008::MCP23008_GPIO_MODE_INPUT = 0xff;
constinit const uint8_t Mcp23008::MCP23008_GPIO_MODE_OUTPUT = 0x00;

constinit const uint8_t Mcp23008::MCP23008_ADR_IODIR = 0x00;
constinit const uint8_t Mcp23008::MCP23008_ADR_IPOL = 0x01;
constinit const uint8_t Mcp23008::MCP23008_ADR_GPINTEN = 0x02;
constinit const uint8_t Mcp23008::MCP23008_ADR_DEFVAL = 0x03;
constinit const uint8_t Mcp23008::MCP23008_ADR_INTCON = 0x04;
constinit const uint8_t Mcp23008::MCP23008_ADR_IOCON = 0x05;
constinit const uint8_t Mcp23008::MCP23008_ADR_GPPU = 0x06;
constinit const uint8_t Mcp23008::MCP23008_ADR_INTF = 0x07;
constinit const uint8_t Mcp23008::MCP23008_ADR_INTCAP = 0x08;
constinit const uint8_t Mcp23008::MCP23008_ADR_GPIO = 0x09;
constinit const uint8_t Mcp23008::MCP23008_ADR_OLAT = 0x0a;
constinit const uint8_t Mcp23008::MCP23008_IOCON_SEQOP = 0x20;
