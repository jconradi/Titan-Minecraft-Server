#pragma once
#include <string>
#include <cstdint>

class MinecraftMap
{
public:
	MinecraftMap();
	MinecraftMap(const std::wstring& mapName) {}
	~MinecraftMap(void);

	const std::wstring& MapName() const { return mapName; }
	int64_t MapSeed() const { return mapSeed; }
	uint8_t MapHeight() const { return worldHeight; }

private:
	enum { DefaultMapHeight = 128 };

	std::wstring mapName;
	int64_t mapSeed;
	uint8_t worldHeight;
};

