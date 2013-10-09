#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <deque>
#include <cstdint>
#include "UserInfo.h"

class MinecraftPacketParser;
class MinecraftPacket;
class OutputPacketStream;

class MinecraftClient : public boost::enable_shared_from_this<MinecraftClient>
{
public:
	typedef boost::shared_ptr<MinecraftClient> Pointer;

	MinecraftClient(boost::asio::io_service& ioService);
	~MinecraftClient(void);
	void Write(OutputPacketStream& packet);
	void StartRead();
	void Disconnect();

	boost::asio::ip::tcp::socket& Socket() { return socket; }
	boost::shared_ptr<UserInfo> User() { return userInfo; }
	boost::shared_ptr<MinecraftPacketParser> Parser() { return parser; }
private:
	enum { DefaultReadSize = 1024 };
	
	void MinecraftClient::HandleRead(boost::system::error_code error, size_t bytes);

	boost::asio::ip::tcp::socket socket;
	bool isActive;
	uint8_t packetData[DefaultReadSize];
	std::deque<uint8_t> data;
	std::vector<boost::shared_ptr<MinecraftPacket> > packets;
	boost::shared_ptr<MinecraftPacketParser> parser;
	boost::shared_ptr<UserInfo> userInfo;
};

