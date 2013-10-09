#include "MinecraftServer.h"
#include <boost/bind.hpp>
#include "MinecraftServerFacade.h"
#include "UserManager.h"

using namespace boost::asio;
using namespace boost::asio::ip;

MinecraftServer::MinecraftServer(io_service& ioService)
	: acceptor(ioService, boost::asio::ip::tcp::endpoint(tcp::endpoint(tcp::v4(), DefaultMinecraftPort)))
{
	isAcceptingConnections = true;
	maxPlayers = 16;
}


MinecraftServer::~MinecraftServer(void)
{
}

void MinecraftServer::Task()
{
	acceptor.get_io_service().poll();
}

void MinecraftServer::Start()
{
	if (isAcceptingConnections)
	{
		MinecraftClient::Pointer client(new MinecraftClient(acceptor.get_io_service()));
		acceptor.async_accept(client->Socket(), boost::bind(&MinecraftServer::HandleAccept, this, client, boost::asio::placeholders::error));
	}
}

void MinecraftServer::HandleAccept(MinecraftClient::Pointer client, const boost::system::error_code& error)
{
	if (!error)
	{
		client->StartRead();
		MinecraftServerFacade::Instance()->GetUserManager()->AddUser(client);
	}
	else

	{
		std::cout << error.message();
	}

	MinecraftClient::Pointer newClient(new MinecraftClient(acceptor.get_io_service()));
	acceptor.async_accept(newClient->Socket(), boost::bind(&MinecraftServer::HandleAccept, this, newClient, boost::asio::placeholders::error));
}