#pragma once
#include <cstdint>
#include <string>
#include <deque>

class InputPacketStream
{
public:
	InputPacketStream();
	~InputPacketStream(void);

	bool IsReadError() const { return readError; }
	bool CanRead(size_t bytes);
	void ReadRaw(uint8_t* dataPtr, size_t bytes);

	void SetStreamData(std::deque<uint8_t>* queue);

	size_t BytesRead() const { return readPos; }
	
	// Read From Stream Methods
	InputPacketStream& operator>>(int8_t& byteData);
	InputPacketStream& operator>>(uint8_t& byteData);
	InputPacketStream& operator>>(bool& boolData);
	InputPacketStream& operator>>(int16_t& shortData);
	InputPacketStream& operator>>(int32_t& intData);
	InputPacketStream& operator>>(int64_t& longData);
	InputPacketStream& operator>>(float& floatData);
	InputPacketStream& operator>>(double& doubleData);
	InputPacketStream& operator>>(std::wstring& stringData);

private:
	std::deque<uint8_t>* data;
	size_t readPos;
	bool readError;
};

