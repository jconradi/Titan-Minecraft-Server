#pragma once
#include <boost/shared_ptr.hpp>
#include <cstdint>

/**
* Server runtime is the main facade/container or director of game server state.
*/
class MinecraftServerRuntime
{
public:
	MinecraftServerRuntime(void);
	~MinecraftServerRuntime(void);

	typedef boost::shared_ptr<MinecraftServerRuntime> Pointer;

	enum MinecraftServerMode
	{
		SurvivalMode,
		CreativeMode
	};

	enum
	{
		TicksPerDay = 24000,
		TicksPerSecond = 20
	};

	int Run();
	MinecraftServerMode ServerMode() const { return serverMode; }
	int32_t WorldTime() const { return worldTime; }
	
private:
	int32_t worldTime;
	MinecraftServerMode serverMode;
};

