//
// Created by koscum on 29/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
#define AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H


#include <functional>
#include <vector>
#include <queue>

class I2cController
{
public:
	enum class State
	{
		READY,
		BUSY,
	};

	class I2cRequest
	{
	public:
		enum class Type
		{
			TX,
			TX_MEM,
			RX,
			RX_MEM,
		};

		virtual ~I2cRequest() noexcept;

		[[nodiscard]] const Type getType() const;

		[[nodiscard]] const uint16_t getAddress() const;

		[[nodiscard]] const uint16_t getRegisterAddress() const;

		virtual void done() = 0;

	protected:
		explicit I2cRequest(Type type,
		                    uint16_t address,
		                    uint16_t registerAddress = 0x00);

		const Type type;
		const uint16_t address;
		const uint16_t registerAddress;
	};

	class I2cRequestTx : public I2cRequest
	{
	public:
		explicit I2cRequestTx(uint16_t address,
		                      std::vector<uint8_t> *data,
		                      const std::function<void()> *callback = nullptr);

		explicit I2cRequestTx(uint16_t address,
		                      uint16_t registerAddress,
		                      std::vector<uint8_t> *data,
		                      const std::function<void()> *callback = nullptr);

		~I2cRequestTx() noexcept override;

		[[nodiscard]] std::vector<uint8_t> *getData() const;

		void done() override;

	private:
		std::vector<uint8_t> *data;
		const std::function<void()> *callback;
	};

	class I2cRequestRx : public I2cRequest
	{
	public:
		explicit I2cRequestRx(uint16_t address,
		                      uint16_t size,
		                      const std::function<void(std::vector<uint8_t> *)> *callback = nullptr);

		explicit I2cRequestRx(uint16_t address,
		                      uint16_t registerAddress,
		                      uint16_t size,
		                      const std::function<void(std::vector<uint8_t> *)> *callback = nullptr);

		~I2cRequestRx() noexcept override;

		[[nodiscard]] const uint16_t getSize() const;

		uint8_t *getBuffer();

		void done() override;

	private:
		const uint16_t size;
		const std::function<void(std::vector<uint8_t> *)> *callback;

		uint8_t *buffer;
	};

	bool request(I2cRequest *i2cRequest);

	void requestCompleted();

	static I2cController *getInstance();

private:
	I2cController();

	void processNextRequest();

	void masterTransmit(uint16_t _address,
	                    std::vector<uint8_t> *_data);

	void memoryWrite(uint16_t _address,
	                 uint16_t _registerAddress,
	                 std::vector<uint8_t> *_data);

	void masterReceive(uint16_t _address,
	                   uint8_t *_buffer,
	                   uint16_t _size);

	void memoryRead(uint16_t _address,
	                uint16_t _registerAddress,
	                uint8_t *_buffer,
	                uint16_t _size);

	std::queue<I2cRequest *> requestQueue;

	static I2cController *instance;

	static const uint16_t REQUEST_QUEUE_LIMIT;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
