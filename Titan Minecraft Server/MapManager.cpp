#include "MapManager.h"
#include "MinecraftMap.h"


MapManager::MapManager(void)
	: loadedMap(new MinecraftMap)
{
}


MapManager::~MapManager(void)
{
}
