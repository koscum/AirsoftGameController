//
// Created by koscum on 29/06/2021.
//

#include <i2c.h>
#include "I2cController.h"
#include "stm32f4xx_hal_i2c.h"

I2cController::I2cRequest::I2cRequest(Type _type,
                                      uint16_t _address,
                                      uint16_t _registerAddress) :
		type(_type),
		address(_address),
		registerAddress(_registerAddress),
		state(State::READY) {}

I2cController::I2cRequest::~I2cRequest() noexcept = default;

I2cController::I2cRequest::Type I2cController::I2cRequest::getType() const
{
	return type;
}

uint16_t I2cController::I2cRequest::getAddress() const
{
	return address;
}

uint16_t I2cController::I2cRequest::getRegisterAddress() const
{
	return registerAddress;
}

I2cController::I2cRequest::State I2cController::I2cRequest::getState() const
{
	return state;
}

void I2cController::I2cRequest::setState(State _state)
{
	state = _state;
}

I2cController::I2cRequestRx::I2cRequestRx(uint16_t _address,
                                          uint16_t _size,
                                          const std::function<void(std::vector<uint8_t> *)> *_callback) :
		I2cRequest(I2cRequest::Type::RX, _address, 0x00),
		buffer(new uint8_t[_size]{0x0}),
		size(_size),
		callback(_callback) {}

I2cController::I2cRequestRx::I2cRequestRx(uint16_t _address,
                                          uint16_t _registerAddress,
                                          uint16_t _size,
                                          const std::function<void(std::vector<uint8_t> *)> *_callback) :
		I2cRequest(I2cRequest::Type::RX_MEM, _address, _registerAddress),
		buffer(new uint8_t[_size]{0x0}),
		size(_size),
		callback(_callback) {}

I2cController::I2cRequestRx::~I2cRequestRx() noexcept
{
	delete[] buffer;
	delete callback;
}

uint16_t I2cController::I2cRequestRx::getSize() const
{
	return size;
}

uint8_t *I2cController::I2cRequestRx::getBuffer()
{
	return buffer;
}

void I2cController::I2cRequestRx::done()
{
	if (callback != nullptr)
	{
		auto data = new std::vector<uint8_t>{};
		for (uint16_t i = 0; i < size; ++i) data->push_back(buffer[i]);

		(*callback)(data);
		delete data;
	}
}

I2cController::I2cRequestTx::I2cRequestTx(uint16_t _address,
                                          std::vector<uint8_t> *_data,
                                          const std::function<void()> *_callback) :
		I2cRequest(I2cRequest::Type::TX,
		           _address,
		           0x00),
		data(_data),
		callback(_callback) {}

I2cController::I2cRequestTx::I2cRequestTx(uint16_t _address,
                                          uint16_t _registerAddress,
                                          std::vector<uint8_t> *_data,
                                          const std::function<void()> *_callback) :
		I2cRequest(I2cRequest::Type::TX_MEM,
		           _address,
		           _registerAddress),
		data(_data),
		callback(_callback) {}

I2cController::I2cRequestTx::~I2cRequestTx() noexcept
{
	delete data;
	delete callback;
}

std::vector<uint8_t> *I2cController::I2cRequestTx::getData() const
{
	return data;
}

void I2cController::I2cRequestTx::done()
{
	if (callback != nullptr) (*callback)();
}

bool I2cController::request(I2cRequest *_request)
{
	if (requestQueue.size() < REQUEST_QUEUE_LIMIT)
	{
		requestQueue.push(_request);

		return true;
	} else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}

	return false;
}

void I2cController::tick()
{
	if (!requestQueue.empty())
	{
		auto request = requestQueue.front();
		switch (request->getState())
		{
			case I2cRequest::State::READY:
			{
				request->setState(I2cRequest::State::IN_PROGRESS);

				switch (request->getType())
				{
					case I2cRequest::Type::RX:
					{
						auto requestRx = reinterpret_cast<I2cRequestRx *>(request);
						masterReceive(requestRx->getAddress(),
						              requestRx->getBuffer(),
						              requestRx->getSize());

						break;
					}
					case I2cRequest::Type::RX_MEM:
					{
						auto requestRx = reinterpret_cast<I2cRequestRx *>(request);
						memoryRead(requestRx->getAddress(),
						           requestRx->getRegisterAddress(),
						           requestRx->getBuffer(),
						           requestRx->getSize());

						break;
					}
					case I2cRequest::Type::TX:
					{
						auto requestTx = reinterpret_cast<I2cRequestTx *>(request);
						masterTransmit(requestTx->getAddress(),
						               requestTx->getData());

						break;
					}
					case I2cRequest::Type::TX_MEM:
					{
						auto requestTx = reinterpret_cast<I2cRequestTx *>(request);
						memoryWrite(requestTx->getAddress(),
						            requestTx->getRegisterAddress(),
						            requestTx->getData());

						break;
					}
				}

				break;
			}
			case I2cRequest::State::COMPLETED:
			{
				requestQueue.pop();

				request->done();
				delete request;

				break;
			}
			default:
				break;
		}
	}
}

void I2cController::requestCompleted()
{
	if (!requestQueue.empty())
	{
		auto request = requestQueue.front();

		request->setState(I2cRequest::State::COMPLETED);
	}
}

void I2cController::masterTransmit(uint16_t _address,
                                   std::vector<uint8_t> *_data)
{
	HAL_I2C_Master_Transmit_DMA(&hi2c1,
	                            _address << 1,
	                            _data->data(),
	                            _data->size());
}

void I2cController::memoryWrite(uint16_t _address,
                                uint16_t _registerAddress,
                                std::vector<uint8_t> *_data)
{
	HAL_I2C_Mem_Write_DMA(&hi2c1,
	                      _address << 1,
	                      _registerAddress,
	                      I2C_MEMADD_SIZE_8BIT,
	                      _data->data(),
	                      _data->size());
}

void I2cController::masterReceive(uint16_t _address,
                                  uint8_t *_buffer,
                                  uint16_t _size)
{
	HAL_I2C_Master_Receive_DMA(&hi2c1,
	                           _address << 1,
	                           _buffer,
	                           _size);
}

void I2cController::memoryRead(uint16_t _address,
                               uint16_t _registerAddress,
                               uint8_t *_buffer,
                               uint16_t _size)
{
	HAL_I2C_Mem_Read_DMA(&hi2c1,
	                     _address << 1,
	                     _registerAddress,
	                     I2C_MEMADD_SIZE_8BIT,
	                     _buffer,
	                     _size);
}

I2cController *I2cController::getInstance()
{
	if (!instance) instance = new I2cController();

	return instance;
}

I2cController::I2cController() = default;

I2cController *I2cController::instance = nullptr;

constinit const uint16_t I2cController::REQUEST_QUEUE_LIMIT = 256;
