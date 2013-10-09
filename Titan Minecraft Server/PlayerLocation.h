#pragma once
#include <cstdint>

class PlayerLocation
{
public:
	PlayerLocation(void) 
	{
		x = y = z = 0.;
		yaw = pitch = 0.f;
		dimension = 0;
		stance = 0.;
		onGround = false;
	}

	double& X()			{ return x; }
	double& Y()			{ return y; }
	double& Z()			{ return z; }
	float& Yaw()		{ return yaw; }
	float& Pitch()		{ return pitch; }
	double& Stance()	{ return stance; }
	bool& OnGround()	{ return onGround; }
	uint8_t& Dimension(){ return dimension; }
private:
	double x;
	double y;
	double z;
	float yaw;
	float pitch;
	double stance;
	bool onGround;
	uint8_t dimension;
};

