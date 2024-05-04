#pragma once
#include <utility/vector.hpp>

namespace ellohim
{
	enum player_skill : std::int32_t
	{
		pyrokinesis = 0,
		sclerokinesis = 1,
		clairvoyance = 2,
		teleportation = 3,
		invisibility = 4,
		electrokinesis = 5,
		duplication = 6,
		hypervelocity = 7,
		psychokinesis = 8,
		psychokinesis_kasane = 9,
		no_skill = 10
	};

	struct FMinimalViewInfo
	{
		Vector3 Location; // 0x00(0x0c)
		Vector3 Rotation; // 0x0c(0x0c)
		float FOV; // 0x18(0x04)
		float DesiredFOV; // 0x1c(0x04)
		float OrthoWidth; // 0x20(0x04)
		float OrthoNearClipPlane; // 0x24(0x04)
		float OrthoFarClipPlane; // 0x28(0x04)
		float AspectRatio; // 0x2c(0x04)
	};

	struct FCameraCacheEntry
	{
		float Timestamp; // 0x00(0x04)
		char pad_4[0xc]; // 0x04(0x0c)
		FMinimalViewInfo POV; // 0x10(0x5e0)
	};
}