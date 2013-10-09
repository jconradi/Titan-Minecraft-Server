#include "MinecraftServerRuntime.h"
#include <ctime>
#include <cstdint>
#include "MinecraftServerFacade.h"
#include "UserManager.h"
#include "MinecraftServer.h"

MinecraftServerRuntime::MinecraftServerRuntime(void)
{
}


MinecraftServerRuntime::~MinecraftServerRuntime(void)
{
}

int MinecraftServerRuntime::Run()
{
	MinecraftServerFacade::Pointer server = MinecraftServerFacade::Instance();

	time_t last = time(NULL);
	for (;;)
	{
		time_t now = time(NULL);
		time_t deltaTime = now - last;
		int32_t deltaTicks = deltaTime * TicksPerSecond;

		worldTime += deltaTicks;

		server->GetServer()->Task();
		server->GetUserManager()->Task(deltaTicks);

		last = now;
	}
}