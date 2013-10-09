#include "UserManager.h"
#include "OutputPacketStream.h"
#include "MinecraftClient.h"
#include "MinecraftPacketParser.h"
#include "MinecraftServerFacade.h"
#include "OutputPacketStream.h"
#include "MinecraftPacketParser.h"
#include "MinecraftServerRuntime.h"
#include "MinecraftServerFacade.h"
#include "MapManager.h"
#include "MinecraftMap.h"
#include "MinecraftServer.h"

UserManager::UserManager(void)
	: entityId(0)
	, keepAliveTimerTicks(0)
{
}


UserManager::~UserManager(void)
{
}

void UserManager::AddUser(boost::shared_ptr<MinecraftClient> client)
{ 
	unauthenticatedClients.insert(client);
}

void UserManager::Handshake(boost::shared_ptr<MinecraftClient> client)
{
	OutputPacketStream packet(MinecraftPacketParser::Handshake);
	std::wstring response = L"-";
	
	// No authentication required at the moment.
	packet << response;
	client->Write(packet);
}

void UserManager::DisconnectUnAuthenticatedUser(boost::shared_ptr<MinecraftClient> client, const std::wstring& reason)
{
	OutputPacketStream packet(MinecraftPacketParser::DisconnectOrKick);
	packet << reason;

	client->Write(packet);

	unauthenticatedClients.erase(client);
	client->Disconnect();
}

void UserManager::DisconnectAuthenticatedUser(const std::wstring& username, const std::wstring& reason)
{
}

void UserManager::ClientDisconnected(boost::shared_ptr<MinecraftClient> client)
{
	if (authenticatedClients.right.find(client) != authenticatedClients.right.end())
	{
		authenticatedClients.right.erase(client);
	}
	else if (unauthenticatedClients.find(client) != unauthenticatedClients.end())
	{
		unauthenticatedClients.erase(client);
	}
}

void UserManager::KeepAlive(boost::shared_ptr<MinecraftClient> client)
{
	client->User()->keepAliveInfo.deltaTicksSinceLastResponse = 0;
}

void UserManager::LoginRequest(MinecraftClient::Pointer client, int32_t protocolVersion, const std::wstring& username, int64_t verification1, int32_t verification2, 
		int8_t verification3, int8_t verification4, uint8_t verification5, uint8_t verification6)
{
	if (protocolVersion != client->Parser()->ProtocolVersion)
	{
		DisconnectUnAuthenticatedUser(client, L"Unmatched protocol version.");
		return;
	}

	if (authenticatedClients.left.find(username) != authenticatedClients.left.end())
	{
		DisconnectUnAuthenticatedUser(client, username + L" is already connected to this server.");
		return;
	}

	if (verification1 || verification2 || verification3 || verification4 || verification5 || verification6)
	{
		DisconnectUnAuthenticatedUser(client, L"Unmatched verification fields.");
		return;
	}

	LoginUnAuthenticatedPlayer(username, client);

	return;
}

void UserManager::UpdatePlayerPositionAndLook(boost::shared_ptr<MinecraftClient> client)
{
	OutputPacketStream packet(MinecraftPacketParser::PlayerPositionAndLook);

	packet << client->User()->Location().X() << client->User()->Location().Y() << client->User()->Location().Stance()
		<< client->User()->Location().Z() << client->User()->Location().Yaw() << client->User()->Location().Pitch()
		<< client->User()->Location().OnGround();

	client->Write(packet);
}

void UserManager::LoginUnAuthenticatedPlayer(const std::wstring& username, boost::shared_ptr<MinecraftClient> client)
{
	MinecraftServerFacade::Pointer server = MinecraftServerFacade::Instance();
	OutputPacketStream packet(MinecraftPacketParser::LoginRequest);

	client->User()->playerEntityId = GetNextEntityId();
	client->User()->Username() = username;

	packet << client->User()->playerEntityId << std::wstring(L"") << server->GetMapManager()->Map()->MapSeed()
		<< (int32_t)server->GetRuntime()->ServerMode() << client->User()->Location().Dimension()
		<< (uint8_t)2 << server->GetMapManager()->Map()->MapHeight() << server->GetServer()->MaxPlayers();

	client->Write(packet);

	// Move user from unauthenticated list to authenticated bi-map.
	unauthenticatedClients.erase(client);
	authenticatedClients.insert(ClientMap::value_type(username, client));

	UpdatePlayerPositionAndLook(client);
}

void UserManager::SendToAllClients(OutputPacketStream& packet)
{
	for (ClientMap::const_iterator it = Users().begin(); it != Users().end(); it++)
	{
		it->right->Write(packet);
	}
}

void UserManager::Task(int32_t deltaTicks)
{
	keepAliveTimerTicks += deltaTicks;
	bool shouldSendKeepAlive = false;
	OutputPacketStream packet(MinecraftPacketParser::KeepAlive);

	if (keepAliveTimerTicks >= KeepAliveTimerTicks)
	{
		shouldSendKeepAlive = true;
		keepAliveTimerTicks = 0;
		int32_t keepAliveId = std::rand();
			
		packet << keepAliveId;
	}
	
	for (ClientMap::const_iterator it = Users().begin(); it != Users().end(); it++)
	{
		MinecraftClient::Pointer client = it->right;

		// Client has timed out.
		if (client->User()->keepAliveInfo.deltaTicksSinceLastResponse >= KeepAliveTimeoutTicks)
		{
			DisconnectAuthenticatedUser(client->User()->Username(), L"Client Timeout.");
		}

		if (shouldSendKeepAlive)
		{
			client->Write(packet);
		}
	}
}