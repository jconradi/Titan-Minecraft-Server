#pragma once
#include <boost/shared_ptr.hpp>

class MinecraftMap;

class MapManager
{
public:
	MapManager(void);
	~MapManager(void);

	boost::shared_ptr<MinecraftMap> Map() { return loadedMap; }

private:
	boost::shared_ptr<MinecraftMap> loadedMap;
};

