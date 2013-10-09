#pragma once
#include <string>
#include <boost/shared_ptr.hpp>

class MinecraftClient;

class ChatManager
{
public:
	ChatManager(void);
	~ChatManager(void);

	void ChatMessage(boost::shared_ptr<MinecraftClient> client, const std::wstring& message);
private:
	void ProcessServerCommand(const std::wstring& command) {}
};

