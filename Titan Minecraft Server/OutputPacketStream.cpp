#include "OutputPacketStream.h"
#include "TextUtility.h"
#include <boost/asio.hpp>

using namespace boost::asio::detail::socket_ops;


OutputPacketStream::OutputPacketStream(uint8_t packetId)
{
	data.push_back(packetId);
}

OutputPacketStream::~OutputPacketStream(void)
{
}

void OutputPacketStream::WriteRaw(const uint8_t* data, size_t size)
{
	this->data.insert(this->data.end(), data, data + size);
}

OutputPacketStream& OutputPacketStream::operator<<(uint8_t data)
{
	WriteRaw(&data, sizeof(uint8_t));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(int8_t data)
{
	WriteRaw(reinterpret_cast<uint8_t*>(&data), sizeof(int8_t));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(bool data)
{
	WriteRaw(reinterpret_cast<uint8_t*>(&data), sizeof(int8_t));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(int16_t data)
{
	data = host_to_network_short(data);
	WriteRaw(reinterpret_cast<uint8_t*>(&data), sizeof(int16_t));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(int32_t data)
{
	data = host_to_network_long(data);
	WriteRaw(reinterpret_cast<uint8_t*>(&data), sizeof(int32_t));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(int64_t data)
{
	data = host_to_network_long(data);
	WriteRaw(reinterpret_cast<uint8_t*>(&data), sizeof(int64_t));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(float floatData)
{
	floatData = host_to_network_long(floatData);
	WriteRaw(reinterpret_cast<uint8_t*>(&floatData), sizeof(float));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(double doubleData)
{
	doubleData = host_to_network_long(doubleData);
	WriteRaw(reinterpret_cast<uint8_t*>(&doubleData), sizeof(doubleData));
	return *this;
}

OutputPacketStream& OutputPacketStream::operator<<(const std::wstring& stringData)
{
	int16_t stringLength = stringData.length();
	*this << stringLength;

	std::string str;
	str.insert(str.begin(), stringData.begin(), stringData.end());

	str = TextUtility::ASCIIToUCS2BE(str);

	WriteRaw(reinterpret_cast<uint8_t*>(const_cast<char *>(str.c_str())), str.length());

	return *this;
}