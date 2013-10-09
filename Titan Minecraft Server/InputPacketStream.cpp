#include "InputPacketStream.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <vector>
#include "TextUtility.h"

using namespace boost::asio::detail::socket_ops;

InputPacketStream::InputPacketStream(void)
	: data(NULL)
	, readPos(0)
{
}


InputPacketStream::~InputPacketStream(void)
{
}

bool InputPacketStream::CanRead(size_t bytes)
{
	if (readPos + bytes > data->size())
	{
		readError = true;
		return false;
	}

	return true;
}

void InputPacketStream::ReadRaw(uint8_t* dataPtr, size_t bytes)
{
	if (!CanRead(bytes))
		return;

	std::copy(data->begin() + readPos, data->begin() + readPos + bytes, dataPtr);
	readPos += bytes;
}

void InputPacketStream::SetStreamData(std::deque<uint8_t>* queue)
{
	data = queue;
	readPos = 0;
	readError = false;
}

InputPacketStream& InputPacketStream::operator>>(int8_t& byteData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&byteData), sizeof(int8_t));
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(uint8_t& byteData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&byteData), sizeof(uint8_t));
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(bool& boolData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&boolData), sizeof(int8_t));
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(int16_t& shortData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&shortData), sizeof(int16_t));
	shortData = network_to_host_short(shortData);
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(int32_t& intData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&intData), sizeof(int32_t));
	intData = network_to_host_long(intData);
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(int64_t& longData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&longData), sizeof(int64_t));
	longData = network_to_host_long(longData);
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(float& floatData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&floatData), sizeof(float));
	floatData = network_to_host_long(floatData);
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(double& doubleData)
{
	ReadRaw(reinterpret_cast<uint8_t *>(&doubleData), sizeof(double));
	doubleData = network_to_host_long(doubleData);
	return *this;
}

InputPacketStream& InputPacketStream::operator>>(std::wstring& stringData)
{
	if (!CanRead(sizeof(int16_t)))
		return *this;

	int16_t stringLength;
	*this >> stringLength;
	stringLength *= sizeof(wchar_t);

	if (!CanRead(stringLength) || !stringLength)
		return *this;

	std::vector<uint8_t> data;
	data.resize(stringLength);

	ReadRaw(reinterpret_cast<uint8_t *>(&data[0]), stringLength);

	for (int i = 0; i < stringLength / 2; i++)
	{
		*(uint16_t *)&data[i] = network_to_host_short(*(uint16_t *)&data[i]);
	}

	return *this;
}
