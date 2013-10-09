#pragma once
#include <vector>
#include <string>
#include <cstdint>

class OutputPacketStream
{
public:
	OutputPacketStream(uint8_t packetId);
	~OutputPacketStream(void);

	void WriteRaw(const uint8_t* data, size_t size);

	OutputPacketStream& operator<<(uint8_t data);
	OutputPacketStream& operator<<(int8_t data);
	OutputPacketStream& operator<<(bool data);
	OutputPacketStream& operator<<(int16_t data);
	OutputPacketStream& operator<<(int32_t data);
	OutputPacketStream& operator<<(int64_t data);
	OutputPacketStream& operator<<(float floatData);
	OutputPacketStream& operator<<(double doubleData);
	OutputPacketStream& operator<<(const std::wstring& stringData);

	size_t Size() const { return data.size(); }
	uint8_t& operator[](size_t index) { return data[index]; }
	
private:
	std::vector<uint8_t> data;
};

