#include "MinecraftClient.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "MinecraftServerFacade.h"
#include "UserManager.h"
#include "MinecraftPacketParser.h"
#include "OutputPacketStream.h"

using namespace boost::asio;

MinecraftClient::MinecraftClient(boost::asio::io_service& ioService)
	: socket(ioService)
	, parser(new MinecraftPacketParser)
	, userInfo(new UserInfo)
{
}

MinecraftClient::~MinecraftClient(void)
{
}

void MinecraftClient::Write(OutputPacketStream& packet)
{
	socket.send(buffer(&packet[0], packet.Size()));
}

void MinecraftClient::StartRead()
{
	socket.async_receive(buffer(packetData, sizeof(packetData)),
		boost::bind(&MinecraftClient::HandleRead, shared_from_this(), placeholders::error, placeholders::bytes_transferred));	
}

void MinecraftClient::HandleRead(boost::system::error_code error, size_t bytes)
{
	// Client DC
	if (error)
	{
		MinecraftServerFacade::Instance()->GetUserManager()->ClientDisconnected(shared_from_this());
		return;
	}

	data.insert(data.end(), packetData, &packetData[bytes]);
	parser->ParsePackets(shared_from_this(), data);

	socket.async_receive(buffer(packetData, sizeof(packetData)),
		boost::bind(&MinecraftClient::HandleRead, shared_from_this(), placeholders::error, placeholders::bytes_transferred));	
}

void MinecraftClient::Disconnect()
{
	socket.close();
}