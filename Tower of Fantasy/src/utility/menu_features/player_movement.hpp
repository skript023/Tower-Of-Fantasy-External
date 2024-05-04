#pragma once
#include "utility/utility.hpp"

namespace ellohim::movement
{
	inline void infinite_dodge(bool activate)
	{
		if (activate)
		{
			if (auto pawn = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x0 }))
			{
				static const int RandBean = g_process->read<int>(pawn + 0x21D0);
				static const int CurBeanCount = g_process->read<int>(pawn + 0x21D4);
				static const int64_t ChangeTime = g_process->read<int64_t>(pawn + 0x21D8);

				g_process->write(pawn + 0x21D0, RandBean);
				g_process->write(pawn + 0x21D4, CurBeanCount);
				g_process->write(pawn + 0x21D8, ChangeTime);
			}
		}
	}

	inline void no_clip(bool activate)
	{
		if (auto no_clip = memory::readPointer(g_pointers->m_local_player, { 0x30, 0x2B0, 0x64 }))
		{
			if (activate)
			{
				g_process->write(no_clip, '\x40');
			}
			else
			{
				g_process->write(no_clip, '\x44');
			}
		}
	}

	inline void infinite_jump(bool activate)
	{
		if (activate)
		{
			auto jump_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x358 });

			if (jump_address)
				g_process->write(jump_address, 0);
		}
	}

	inline Vector3 get_entity_coords()
	{
		if (auto player_pos = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x270, 0x2A0, 0x0 }))//{ 0x9F4, 0xC8, 0x0 }
		{
			Vector3 result = g_process->read<Vector3>(player_pos + 0x124);
			
			return result;
		}

		return Vector3(0.f, 0.f, 0.f);
	}

	inline void set_entity_coords(Vector3 coords)
	{
		if (auto player_pos = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x270, 0x2A0, 0x0 }))//{ 0x9F4, 0xC8, 0x0 }
		{
			LOG(INFO) << "Writing Coordinates : x : " << coords.x << " y : " << coords.y << " z : " << coords.z;
			g_process->write(player_pos + 0x124, coords.x);
			g_process->write(player_pos + 0x128, coords.y);
			g_process->write(player_pos + 0x12C, coords.z);
		}
	}

	inline float get_player_movement_speed()
	{
		auto movement_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x298, 0x19C });

		if (movement_speed)
			return g_process->read<float>(movement_speed);

		return 0.f;
	}

	inline float get_player_crouch_speed()
	{
		auto crouch_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x298, 0x1A0 });

		if (crouch_speed)
			return g_process->read<float>(crouch_speed);

		return 0.f;
	}

	inline void set_player_movement_speed(float speed)
	{//auto movement_speed = memory::readPointer(g_pointers->m_world, { 0x30, 0xC58, 0xF0, 0x19C });
		auto movement_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x298, 0x19C });

		if (movement_speed)
			g_process->write(movement_speed, speed);

	}

	inline void set_player_crouch_speed(float speed)
	{//auto crouch_speed = memory::readPointer(g_pointers->m_world, { 0x5A4, 0x48, 0x8, 0x170 });
		auto crouch_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x298, 0x1A0 });

		if (crouch_speed)
			g_process->write(crouch_speed, speed);
	}

	inline float get_player_swim_speed()
	{
		auto swim_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x25E0 });

		if (swim_speed)
			return g_process->read<float>(swim_speed);

		return 0.f;
	}

	inline void set_player_swim_speed(float speed)
	{
		auto swim_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x25E0 });

		if (swim_speed)
			g_process->write(swim_speed, speed);
	}

	inline void set_player_fly_speed(float speed)
	{
		auto fly_speed = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x298, 0x1A8 });

		if (fly_speed)
			g_process->write(fly_speed, speed);
	}

	inline float get_player_gravity()
	{
		auto gravity = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x25B8 });

		if (gravity)
			return g_process->read<float>(gravity);

		return 0.f;
	}

	inline void set_player_gravity(float speed)
	{
		auto gravity = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x25B8 });

		if (gravity)
			g_process->write(gravity, speed);
	}

}