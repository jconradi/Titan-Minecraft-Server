#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "MinecraftClient.h"
#include <cstdint>

/**
* Main manager of remote communications.  Manages network stack for application.
*/
class MinecraftServer
{
public:
	MinecraftServer(boost::asio::io_service& ioService);
	~MinecraftServer(void);

	void Start();

	uint8_t MaxPlayers() const { return maxPlayers; }

	void Task();

private:
	enum { DefaultMinecraftPort = 25565 };
	uint8_t maxPlayers;
	bool isAcceptingConnections;
	boost::asio::ip::tcp::acceptor acceptor;

	void HandleAccept(MinecraftClient::Pointer client, const boost::system::error_code& error);
};

