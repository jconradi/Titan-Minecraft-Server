#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class MinecraftServerRuntime;
class MinecraftServer;
class UserManager;
class ChatManager;
class MapManager;

class MinecraftServerFacade
{
public:
	typedef boost::shared_ptr<MinecraftServerFacade> Pointer;
	static Pointer Instance();
	static void Free();

	void StartServer();
	void StopServer();

	boost::shared_ptr<MinecraftServerRuntime> GetRuntime() const { return runtime; }
	boost::shared_ptr<MinecraftServer> GetServer() const { return server; }
	boost::shared_ptr<UserManager> GetUserManager() const { return userManager; }
	boost::shared_ptr<ChatManager> GetChatManager() const { return chatManager; }
	boost::shared_ptr<MapManager> GetMapManager() const { return mapManager; }

	~MinecraftServerFacade(void);
private:
	MinecraftServerFacade();

	boost::shared_ptr<MinecraftServerRuntime> runtime;
	boost::shared_ptr<MinecraftServer> server;
	boost::shared_ptr<UserManager> userManager;
	boost::shared_ptr<ChatManager> chatManager;
	boost::shared_ptr<MapManager> mapManager;
	boost::asio::io_service ioService;

	static Pointer instance;
};

