#include "MinecraftMap.h"
#include "NBTFile.h"


MinecraftMap::MinecraftMap(void)
	: worldHeight(DefaultMapHeight)
{
	mapSeed = rand();
	NBTFile file("level.dat");
}


MinecraftMap::~MinecraftMap(void)
{
}
