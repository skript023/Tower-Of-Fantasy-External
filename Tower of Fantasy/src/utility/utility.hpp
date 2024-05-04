#pragma once
#include "pointers.hpp"
#include "joaat.hpp"
#include "memory.hpp"
#include "memory/process.hpp"
#include "vector.hpp"
#include <utility/enums.hpp>
#include <Overlay.h>
#include <script.hpp>

namespace ellohim::utility
{
	inline std::string get_name_from_fname(int key)
	{
		uint32_t chunkOffset = (uint32_t)((int)key >> 16); // Block - Sidenote: The casting may not be necessary, arithmetic/logical shifting nonsense.
		auto nameOffset = (unsigned short)key;

		// The first chunk/shard starts at 0x10, so even if chunkOffset is zero, we will start there.
		uint64_t namePoolChunk = g_process->read<uint64_t>(g_pointers->m_name + ((chunkOffset + 2) * 8));
		uint64_t entryOffset = namePoolChunk + (ULONG)(2 * nameOffset);
		auto nameEntry = g_process->read<int16_t>(entryOffset);

		// If this is negative (and the namePoolChunk & entryOffset's are valid then read the string as Unicode
		// I haven't come across this use case yet.
		// e.g. Read<UnicodeString>(entryOffset + 2, -nameLength * 2)
		auto nameLength = nameEntry >> 6;
		//LOG(INFO) << " chunk " << HEX_TO_UPPER(namePoolChunk) << " entry " << HEX_TO_UPPER(entryOffset);
		char result[1028];
		if (nameLength > 0 && nameLength < 512)
		{
			g_process->read_raw(entryOffset + 2, nameLength, &result);
			result[nameLength] = '\0';
			return std::string(result);
		}
		return "";
	}

	inline D3DXMATRIX Matrix(Vector3 rot, Vector3 origin)
	{
		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

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

	inline Vector3 world_to_screen(Vector3 WorldLocation) 
	{
		Vector3 Screenlocation = Vector3{ 0, 0, 0 };

		auto CameraManager = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x2C8, 0x0 });
		FCameraCacheEntry CameraCache = g_process->read<FCameraCacheEntry>(CameraManager + 0x1AF0);

		D3DMATRIX tempMatrix = Matrix(CameraCache.POV.Rotation, Vector3{ 0, 0, 0 });

		Vector3 vAxisX, vAxisY, vAxisZ;

		vAxisX = Vector3{ tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2] };
		vAxisY = Vector3{ tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2] };
		vAxisZ = Vector3{ tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2] };

		float x = WorldLocation.x - CameraCache.POV.Location.x;
		float y = WorldLocation.y - CameraCache.POV.Location.y;
		float z = WorldLocation.z - CameraCache.POV.Location.z;

		Vector3 vDelta = {x, y, z};
		Vector3 vTransformed = Vector3 { 
			(vDelta.x * vAxisX.x) + (vDelta.y + vAxisX.y) + (vDelta.z + vAxisX.z),
			(vDelta.x * vAxisY.x) + (vDelta.y + vAxisY.y) + (vDelta.z + vAxisY.z),
			(vDelta.x * vAxisZ.x) + (vDelta.y + vAxisZ.y) + (vDelta.z + vAxisZ.z)
		};

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		float FovAngle = CameraCache.POV.FOV * CameraCache.POV.AspectRatio;

		float ScreenCenterX = *g_pointers->m_screen_x / 2.0f;
		float ScreenCenterY = *g_pointers->m_screen_y / 2.0f;

		Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

		return Screenlocation;
	}

	inline bool game_state()
	{
		int game_state = 0;
		if (auto state = memory::readPointer(g_pointers->m_world, { 0x128, 0x0 }))
		{
			game_state = g_process->read<int>(state + 0x8);
		}

		return (g_process->read<uintptr_t>(g_pointers->m_world) + 0x128) != NULL;
	}

	inline bool wait_game()
	{
		int game_state = 0;
		if (auto state = memory::readPointer(g_pointers->m_world, { 0x128, 0x0 }))
		{
			game_state = g_process->read<int>(state + 0x8);
		}

		return game_state == 8;
	}

	inline void esp(bool activate)
	{
		if (activate)
		{
			auto level_array = g_process->read<uintptr_t>(g_pointers->m_world) + 0x140;
			auto level_size = memory::read<int>(g_pointers->m_world, { 0x148 });

			for (int j = 0; j < level_size; j++)
			{
				auto first_level = g_process->read<uintptr_t>(level_array) + (j * 0x8);
				if (first_level == NULL) continue;

				auto actor_array = g_process->read<uintptr_t>(first_level) + 0x98;

				int actor_count = memory::read<int>(first_level, { 0xA0 });

				for (int i = 0; i < actor_count; ++i)
				{
					uintptr_t actor = g_process->read<uintptr_t>(actor_array) + i * 0x8;
					if (actor == NULL) continue;

					if (int id = memory::read<int>(actor, { 0x18 }))
					{

						std::string name = get_name_from_fname(id);

						if (name.find("Scene_Box_Refresh_Wild_") != std::string::npos ||
							name.find("BP_Harvest_Gem_") != std::string::npos ||
							name.find("Box_OnlyOnce_") != std::string::npos
							)
						{
							uint64_t root_component = memory::readPointer(actor, { 0x138, 0x0 });
							if (root_component == NULL) continue;

							Vector3 relativelocation = g_process->read<Vector3>(root_component + 0x124);

							Vector3 cizpic = world_to_screen(relativelocation);
							overlay::RGBA White = { 255.f,255.f,255.f,255.f };
							overlay::RGBA Red = { 255.f,0.f,0.f,255.f };
							overlay::DrawLine(0, 0, cizpic.x, cizpic.y, &Red, 0);
							overlay::DrawStrokeText(cizpic.x, cizpic.y, &White, name.c_str());
						}
					}
				}
			}
		}
	}
}