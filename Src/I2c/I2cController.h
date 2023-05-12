#ifndef AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
#define AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H


#include <functional>
#include <vector>
#include <queue>

class I2cController
{
public:
	enum class Status
	{
		OK,
		ERROR,
		BUSY,
		TIMEOUT,
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

		enum class State
		{
			READY,
			IN_PROGRESS,
			COMPLETED,
			ERROR,
		};

		virtual ~I2cRequest() noexcept;

		[[nodiscard]] auto getType() const -> Type;

		[[nodiscard]] auto getAddress() const -> uint16_t;

		[[nodiscard]] auto getRegisterAddress() const -> uint16_t;

		[[nodiscard]] auto getState() const -> State;

		auto setState(State state) -> void;

		virtual auto done() -> void = 0;

	protected:
		explicit I2cRequest(Type type,
		                    uint16_t address,
		                    uint16_t registerAddress = 0x00);

		const Type type;
		const uint16_t address;
		const uint16_t registerAddress;

		State state;
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

		[[nodiscard]] auto getData() const -> std::vector<uint8_t> *;

		auto done() -> void override;

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

		[[nodiscard]] auto getSize() const -> uint16_t;

		[[nodiscard]] auto getBuffer() -> uint8_t *;

		auto done() -> void override;

	private:
		const uint16_t size;
		const std::function<void(std::vector<uint8_t> *)> *callback;

		uint8_t *buffer;
	};

	I2cController(const I2cController &) = delete;

	auto operator=(const I2cController &) -> I2cController & = delete;

	auto request(I2cRequest *i2cRequest) -> bool;

	auto tick() -> void;

	auto requestCompleted(uint8_t error) -> void;

	[[nodiscard]] static auto getInstance() -> I2cController *;

private:
	I2cController();

	auto masterTransmit(uint16_t _address,
	                    std::vector<uint8_t> *_data) -> Status;

	auto memoryWrite(uint16_t _address,
	                 uint16_t _registerAddress,
	                 std::vector<uint8_t> *_data) -> Status;

	auto masterReceive(uint16_t _address,
	                   uint8_t *_buffer,
	                   uint16_t _size) -> Status;

	auto memoryRead(uint16_t _address,
	                uint16_t _registerAddress,
	                uint8_t *_buffer,
	                uint16_t _size) -> Status;

	std::queue<I2cRequest *> requestQueue;

	static I2cController *instance;

	static const uint16_t REQUEST_QUEUE_LIMIT;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
