#pragma once
#include <set>
#include <string>
#include <cstdint>
#include <boost/bimap.hpp>
#include <boost/shared_ptr.hpp>

class MinecraftClient;
class OutputPacketStream;

class UserManager
{
public:
	UserManager(void);
	~UserManager(void);

	typedef boost::bimap<std::wstring, boost::shared_ptr<MinecraftClient>> ClientMap;

	void AddUser(boost::shared_ptr<MinecraftClient> client);
	void Handshake(boost::shared_ptr<MinecraftClient> client);
	void DisconnectUnAuthenticatedUser(boost::shared_ptr<MinecraftClient> client, const std::wstring& reason);
	void DisconnectAuthenticatedUser(const std::wstring& username, const std::wstring& reason);
	void ClientDisconnected(boost::shared_ptr<MinecraftClient> client);
	void KeepAlive(boost::shared_ptr<MinecraftClient> client);
	void LoginRequest(boost::shared_ptr<MinecraftClient> client, int32_t protocolVersion, const std::wstring& username, int64_t verification1, int32_t verification2, 
		int8_t verification3, int8_t verification4, uint8_t verification5, uint8_t verification6);
	void UpdatePlayerPositionAndLook(boost::shared_ptr<MinecraftClient> client);
	boost::shared_ptr<MinecraftClient> GetClient(const std::wstring& username);

	void SendToAllClients(OutputPacketStream& packet);
	void SendToAllClientsExcept(const std::wstring& username, OutputPacketStream& packet);

	uint32_t TotalPlayers() const { return authenticatedClients.size(); }
	ClientMap& Users() { return authenticatedClients; }

	void Task(int32_t deltaTicks);

private:
	int32_t entityId;
	ClientMap authenticatedClients;
	std::set<boost::shared_ptr<MinecraftClient> > unauthenticatedClients;
	
	int32_t keepAliveTimerTicks;

	int32_t GetNextEntityId() { return entityId++; }
	// Promotes an unauthenticated to authenticated status.
	void LoginUnAuthenticatedPlayer(const std::wstring& username, boost::shared_ptr<MinecraftClient> client);

	enum
	{
		KeepAliveTimeoutTicks = 1200,
		// How many in game ticks to wait before sending out a new keep-alive
		KeepAliveTimerTicks = 500
	};
};

