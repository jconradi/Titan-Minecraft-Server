#pragma once
#include <string>
#include <cstdint>
#include "KeepAliveInfo.h"
#include "PlayerLocation.h"

class UserInfo
{
public:
	int32_t playerEntityId;
	KeepAliveInfo keepAliveInfo;
	
	// 0 or less = dead, 20 = full HP
	int32_t health;
	// 0 - 20
	int32_t food;
	// Varies from 0.0 to 5.0 increments
	float foodSaturation;

	PlayerLocation& Location() { return playerLocation; }
	std::wstring& Username() { return username; }

private:
	PlayerLocation playerLocation;
	std::wstring username;
};