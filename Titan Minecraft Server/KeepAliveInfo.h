#pragma once
#include <cstdint>

struct KeepAliveInfo
{
	int32_t lastKeepAlive;
	// Server game ticks since we've last received a response from this client.
	int32_t deltaTicksSinceLastResponse;
};