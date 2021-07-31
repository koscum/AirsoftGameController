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
		registerAddress(_registerAddress) {}

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
	delete buffer;
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

bool I2cController::request(I2cRequest *_i2cRequest)
{
	if (requestQueue.size() < REQUEST_QUEUE_LIMIT)
	{
		requestQueue.push(_i2cRequest);

		if (requestQueue.size() == 1) processNextRequest();

		return true;
	} else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}

	return false;
}

void I2cController::requestCompleted()
{
	auto i2cRequest = requestQueue.front();
	requestQueue.pop();

	i2cRequest->done();
	delete i2cRequest;

	if (!requestQueue.empty()) processNextRequest();
}

void I2cController::processNextRequest()
{
	if (!requestQueue.empty())
	{
		switch (requestQueue.front()->getType())
		{
			case I2cRequest::Type::RX:
			{
				auto i2cPackageRx = reinterpret_cast<I2cRequestRx *>(requestQueue.front());
				masterReceive(i2cPackageRx->getAddress(),
				              i2cPackageRx->getBuffer(),
				              i2cPackageRx->getSize());

				break;
			}
			case I2cRequest::Type::RX_MEM:
			{
				auto i2cPackageRx = reinterpret_cast<I2cRequestRx *>(requestQueue.front());
				memoryRead(i2cPackageRx->getAddress(),
				           i2cPackageRx->getRegisterAddress(),
				           i2cPackageRx->getBuffer(),
				           i2cPackageRx->getSize());

				break;
			}
			case I2cRequest::Type::TX:
			{
				auto i2cPackageTx = reinterpret_cast<I2cRequestTx *>(requestQueue.front());
				masterTransmit(i2cPackageTx->getAddress(),
				               i2cPackageTx->getData());

				break;
			}
			case I2cRequest::Type::TX_MEM:
			{
				auto i2cPackageTx = reinterpret_cast<I2cRequestTx *>(requestQueue.front());
				memoryWrite(i2cPackageTx->getAddress(),
				            i2cPackageTx->getRegisterAddress(),
				            i2cPackageTx->getData());

				break;
			}
		}
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
