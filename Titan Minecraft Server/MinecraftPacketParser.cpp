#include "MinecraftPacketParser.h"
#include "UserManager.h"
#include "MinecraftServerFacade.h"
#include "MinecraftClient.h"
#include "OutputPacketStream.h"
#include "InputPacketStream.h"
#include "ChatManager.h"
#include "MinecraftServer.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <iomanip>


MinecraftPacketParser::MinecraftPacketParser()
	: inputStream(new InputPacketStream)
{
}


MinecraftPacketParser::~MinecraftPacketParser(void)
{
}

void MinecraftPacketParser::ParsePackets(MinecraftClient::Pointer client, std::deque<uint8_t>& dataStream)
{
	boost::shared_ptr<MinecraftServerFacade> server = MinecraftServerFacade::Instance();
	InputPacketStream& inputStream = *this->inputStream;
	
	for (;;)
	{
		// Make sure to reset read ptr at each iteration.
		inputStream.SetStreamData(&dataStream);
		int8_t commandByte;
		inputStream >> commandByte;

		if (inputStream.IsReadError())
			return;

		char szDat[16];
		sprintf(szDat, "%X", commandByte);
		std::cout << "Packet ID: " << szDat << std::endl;

		/* Giant switch through all packet types.  Dispatch to appropriate managers */
		switch ((uint8_t)commandByte)
		{
		case KeepAlive:
			{
				int32_t keepAliveId;
				inputStream >> keepAliveId;

				if (inputStream.IsReadError())
					return;

				server->GetUserManager()->KeepAlive(client);
			}
			break;
		case LoginRequest:
			{
				int32_t protocolVersion;
				std::wstring username;

				// Unused space for future protocol
				int64_t verification1;
				int32_t verification2;
				int8_t verification3, verification4;
				uint8_t verification5, verification6;

				inputStream >> protocolVersion >> username >> verification1 >>
					verification2 >> verification3 >> verification4 >> verification5 >> verification6;

				if (inputStream.IsReadError())
					return;

				server->GetUserManager()->LoginRequest(client, protocolVersion, username, verification1, verification2, verification3,
					verification4, verification5, verification6);
			}
			break;
		case Handshake:
			{
				std::wstring username;

				inputStream >> username;

				if (inputStream.IsReadError())
					return;

				server->GetUserManager()->Handshake(client);
			}
			break;
		case ChatMessage:
			{
				std::wstring message;

				inputStream >> message;

				if (inputStream.IsReadError())
					return;

				server->GetChatManager()->ChatMessage(client, message);
			}
			break;
		case TimeUpdate:
			{
				int64_t time;

				inputStream >> time;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case EntityEquipment:
			{
				int32_t entityId;
				int16_t slot, itemId, damage;

				inputStream >> entityId >> slot >> itemId >> damage;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case UseEntity:
			{
				int32_t user, target;
				bool isLeftClick;

				inputStream >> user >> target >> isLeftClick;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case Respawn:
			{
				uint8_t world, difficulty;
				bool isCreativeMode;
				int16_t worldHeight;
				int64_t mapSeed;

				inputStream >> world >> difficulty >> isCreativeMode >> worldHeight >> mapSeed;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case Player:
			{
				bool isOnGround;

				inputStream >> isOnGround;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PlayerPosition:
			{
				double x, y, stance, z;
				bool isOnGround;

				inputStream >> x >> y >> stance >> z >> isOnGround;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PlayerLook:
			{
				float yaw, pitch;
				bool isOnGround;

				inputStream >> yaw >> pitch >> isOnGround;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PlayerPositionAndLook:
			{
				double x, y, stance, z;
				float yaw, pitch;
				bool isOnGround;

				inputStream >> x >> y >> stance >> z >> yaw >> pitch >> isOnGround;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PlayerDigging:
			{
				int8_t status;
				int32_t x, z;
				int8_t y;
				int8_t face;

				inputStream >> status >> x >> y >> z >> face;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PlayerBlockPlacement:
			{
				int32_t x;
				int8_t y;
				int32_t z;
				int8_t direction;
				int16_t blockId;
				int8_t amount;
				int16_t damage;

				inputStream >> x >> y >> z >> direction >> blockId;
				
				if (blockId >= 0)
					inputStream >> amount >> damage;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case HoldingChange:
			{
				int16_t slotId;

				inputStream >> slotId;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case Animation:
			{
				int32_t eid;
				int8_t animation;

				inputStream >> eid >> animation;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case EntityAction:
			{
				int32_t eid;
				int8_t actionId;

				inputStream >> eid >> actionId;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PickupSpawn:
			{
				int32_t eid;
				int16_t item;
				int8_t count;
				int16_t damageOrData;
				int32_t x, y, z;
				int8_t rotation, pitch, roll;

				inputStream >> eid >> item >> count >> damageOrData >> x >> y >> z
					>> rotation >> pitch >> roll;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case EntityPainting:
			{
				int32_t eid;
				std::wstring title;
				int32_t x, y, z, direction;

				inputStream >> eid >> title >> x >> y >> z >> direction;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case ExperienceOrb:
			{
				int32_t eid, x, y, z;
				int16_t count;

				inputStream >> eid >> x >> y >> z >> count;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case StanceUpdate:
			{
				float unkwn1, unkwn2, unkwn3, unkwn4;
				bool unkwn5, unkwn6;

				inputStream >> unkwn1 >> unkwn2 >> unkwn3 >> unkwn4 >> unkwn5 >> unkwn6;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case EntityVelocity:
			{
				int32_t eid;
				int16_t velx, vely, velz;

				inputStream >> eid >> velx >> vely >> velz;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case AttachEntity:
			{
				int32_t eid, vehicleId;

				inputStream >> eid >> vehicleId;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case EntityMetaData:
			{
				//TODO: Entity Metadata requires metadata parsing.
			}
			break;
		case EntityEffect:
			{
				int32_t eid;
				int8_t effectId, amplifier;
				int16_t duration;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case RemoveEntityEffect:
			{
				int32_t eid;
				int8_t effectId;

				inputStream >> eid >> effectId;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case Experience:
			{
				int8_t currentExperience, level;
				int16_t totalExperience;

				inputStream >> currentExperience >> level >> totalExperience;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case MultiBlockChange:
			{
				// TODO:  Packet requires metadata.
			}
			break;
		case BlockChange:
			{
				// TODO: Packet requires metadata.
			}
			break;
		case Explosion:
			{
				double x, y, z;
				float unkn1;
				int32_t recordCount;
				int8_t byte;

				inputStream >> x >> y >> z >> unkn1 >> recordCount;

				for (int i = 0; i < recordCount; i++)
					for (int j = 0; j < 3; j++)
						inputStream >> byte;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case SoundEffect:
			{
				int32_t eid;
				int32_t x;
				int8_t y;
				int32_t z, soundData;

				inputStream >> eid >> x >> y >> z >> soundData;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case NewState:
			{
				int8_t reason, gameMode;

				inputStream >> reason >> gameMode;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case Thunderbolt:
			{
				int32_t eid;
				bool unkwn1;
				int32_t x, y , z;

				inputStream >> eid >> unkwn1 >> x >> y >> z;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case OpenWindow:
			{
				int8_t windowId, inventoryType;
				std::wstring windowTitle;
				int8_t slots;

				inputStream >> windowId >> inventoryType >> windowTitle >> slots;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case CloseWindow:
			{
				int8_t windowId;

				inputStream >> windowId;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case WindowClick:
			{
				int8_t windowId;
				int16_t slot;
				int8_t rightClick;
				int16_t actionNumber;
				bool isShift;
				int16_t itemId;
				int8_t itemCount;
				int16_t itemUses;

				inputStream >> windowId >> slot >> rightClick >> actionNumber >> isShift >> itemId;

				if (itemId != -1)
					inputStream >> itemCount >> itemUses;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case Transaction:
			{
				int8_t windowId;
				int16_t actionNumber;
				bool accepted;

				inputStream >> windowId >> actionNumber >> accepted;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case CreativeInventoryAction:
			{
				int16_t slot, itemId, quantity, damage;

				inputStream >> slot >> itemId >> quantity >> damage;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case UpdateSign:
			{
				int32_t x;
				int16_t y;
				int32_t z;
				std::wstring text1, text2, text3, text4;

				inputStream >> x >> y >> z >> text1 >> text2 >> text3 >> text4;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case IncrementStatistic:
			{
				int32_t statId;
				int8_t amount;

				inputStream >> statId >> amount;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case PlayerListItem:
			{
				std::wstring playerName;
				bool online;
				int16_t ping;

				inputStream >> playerName >> online >> ping;

				if (inputStream.IsReadError())
					return;
			}
			break;
		case ServerListPing:
			{
				std::wstring response;
				
				response = L"Alpha Server. ";
				response += L"\xA7";
				response += server->GetUserManager()->TotalPlayers();
				response += L"\xA7";
				response += server->GetServer()->MaxPlayers();

				server->GetUserManager()->DisconnectUnAuthenticatedUser(client, L"Alpha Server.");
			}
			break;
		default:
			{
				std::cout << "unknown pkt ID: " << std::hex << (short)commandByte << std::endl;
			}
			break;
		}

		if (!inputStream.IsReadError())
		{
			dataStream.erase(dataStream.begin(), dataStream.begin() + inputStream.BytesRead());
		}
	}
}