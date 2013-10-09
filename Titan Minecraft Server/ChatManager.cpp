#include "ChatManager.h"
#include "MinecraftClient.h"
#include "MinecraftServerFacade.h"
#include "UserManager.h"
#include "OutputPacketStream.h"
#include "MinecraftPacketParser.h"

ChatManager::ChatManager(void)
{
}


ChatManager::~ChatManager(void)
{
}

void ChatManager::ChatMessage(boost::shared_ptr<MinecraftClient> client, const std::wstring& message)
{
	OutputPacketStream packet(MinecraftPacketParser::ChatMessage);

	if (message[0] == L'/')
	{
		ProcessServerCommand(message);
		return;
	}

	std::wstring chatMessage = L"<" + client->User()->Username() + L">" + message;

	packet << message;

	MinecraftServerFacade::Instance()->GetUserManager()->SendToAllClients(packet);
}