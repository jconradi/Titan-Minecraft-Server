#include "MinecraftServerFacade.h"
#include "UserManager.h"
#include "ChatManager.h"
#include "MinecraftServer.h"
#include "MinecraftServerRuntime.h"
#include "MapManager.h"

MinecraftServerFacade::Pointer MinecraftServerFacade::instance = MinecraftServerFacade::Pointer();

MinecraftServerFacade::Pointer MinecraftServerFacade::Instance()
{
	if (instance == NULL)
		instance = Pointer(new MinecraftServerFacade);
	return instance;
}

void MinecraftServerFacade::Free()
{
	if (instance != NULL)
	{
		instance.reset();
	}
}

MinecraftServerFacade::MinecraftServerFacade(void)
	: userManager(new UserManager)
	, chatManager(new ChatManager)
	, mapManager(new MapManager)
{
}

MinecraftServerFacade::~MinecraftServerFacade(void)
{
	StopServer();
}

void MinecraftServerFacade::StartServer()
{
	server = boost::shared_ptr<MinecraftServer>(new MinecraftServer(ioService));
	runtime = boost::shared_ptr<MinecraftServerRuntime>(new MinecraftServerRuntime);

	server->Start();

	for (;;)
	{
		runtime->Run();
	}
	
}

void MinecraftServerFacade::StopServer()
{
}