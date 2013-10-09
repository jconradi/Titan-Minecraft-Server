#include <iostream>
#include "MinecraftServerFacade.h"

int main(int argc, char** argv)
{
	MinecraftServerFacade::Pointer server = MinecraftServerFacade::Instance();

	// Blocks until exit.
	server->StartServer();
	
	return 0;
}