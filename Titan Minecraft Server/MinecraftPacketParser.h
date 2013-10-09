#pragma once
#include <boost/shared_ptr.hpp>
#include <deque>
#include <cstdint>

class MinecraftClient;
class InputPacketStream;

class MinecraftPacketParser
{
public:
	MinecraftPacketParser();
	virtual ~MinecraftPacketParser(void);

	void ParsePackets(boost::shared_ptr<MinecraftClient> client, std::deque<uint8_t>& dataStream);
	enum
	{
		ProtocolVersion = 17
	};

	enum PacketID
	{
		KeepAlive,
		LoginRequest,
		Handshake,
		ChatMessage,
		TimeUpdate,
		EntityEquipment,
		SpawnPosition,
		UseEntity,
		UpdateHealth,
		Respawn,
		Player,
		PlayerPosition,
		PlayerLook,
		PlayerPositionAndLook,
		PlayerDigging,
		PlayerBlockPlacement,
		HoldingChange,
		UseBed,
		Animation,
		EntityAction,
		NamedEntitySpawn,
		PickupSpawn,
		CollectItem,
		AddObjectOrVehicle,
		MobSpawn,
		EntityPainting,
		ExperienceOrb,
		StanceUpdate,
		EntityVelocity,
		DestroyEntity,
		Entity,
		EntityRelativeMove,
		EntityLook,
		EntityLookAndRelativeMove,
		EntityTeleport,
		EntityStatus,
		AttachEntity,
		EntityMetaData,
		EntityEffect = 0x29,
		RemoveEntityEffect = 0x2A,
		Experience = 0x2B,
		PreChunk = 0x32,
		MapChunk = 0x33,
		MultiBlockChange = 0x34,
		BlockChange = 0x35,
		BlockAction = 0x36,
		Explosion = 0x3C,
		SoundEffect = 0x3D,
		NewState = 0x46,
		Thunderbolt = 0x47,
		OpenWindow = 0x64,
		CloseWindow = 0x65,
		WindowClick = 0x66,
		SetSlot = 0x67,
		WindowItems = 0x68,
		UpdateProgressBar = 0x69,
		Transaction = 0x6A,
		CreativeInventoryAction = 0x6B,
		UpdateSign = 0x82,
		ItemData = 0x83,
		IncrementStatistic = 0xC8,
		PlayerListItem = 0xC9,
		ServerListPing = 0xFE,
		DisconnectOrKick = 0xFF
	};
private:
	boost::shared_ptr<InputPacketStream> inputStream;

	enum
	{
		PacketIDLength = 1,
	};

};

