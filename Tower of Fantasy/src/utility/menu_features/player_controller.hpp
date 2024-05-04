#pragma once
#include "utility/utility.hpp"

namespace ellohim::misc
{
	inline void game_speed(Vector3 scale)
	{	//Delation Address 0xA0 && delation check address 0x1A60
		if (auto speed_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x138, 0x13C }))
		{
			g_process->write(speed_address, scale);
		}
	}

	inline bool is_crouch()
	{
		if (auto is_crouch_addr = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x340 }))
			return g_process->read<float>(is_crouch_addr);

		return false;
	}

	inline bool is_armor_broken()
	{
		if (auto is_armor_broken = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x62C }))
			return g_process->read<float>(is_armor_broken);

		return false;
	}

	inline void game_speed(float speed)
	{	//Delation Address 0xA0 && delation check address 0x1A60
		if (auto speed_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x1A60 }))
		{
			g_process->write(speed_address, speed);
		}
	}

	inline int get_player_level()
	{
		if (auto level_addr = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x690 }))
		{
			return g_process->read<int>(level_addr);
		}

		return 0;
	}

	inline void set_player_level(int level)
	{
		if (auto level_addr = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x690 }))
		{
			g_process->write(level_addr, level);
		}
	}

	inline void godmode(bool activate)
	{
		if (auto godmode_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x61 }))
		{
			uint8_t godmode_status = g_process->read<uint8_t>(godmode_address);
			if (activate && godmode_status == 141)
			{
				g_process->write(godmode_address, (uint8_t)13);
			}
			else if (activate && godmode_status == 13)
			{
				g_process->write(godmode_address, (uint8_t)141);
			}
		}
	}

	inline void freeze_mobs(bool activate)
	{
		if (auto freeze = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x13E7 }))
		{
			uint8_t freeze_status = g_process->read<bool>(freeze);
			if (activate && !freeze_status)
			{
				g_process->write(freeze, true);
			}
			else if (activate && freeze_status)
			{
				g_process->write(freeze, false);
			}
		}
	}

	inline float get_attack_multiplier()
	{
		if (auto attack_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x788, 0x28C }))
		{
			return g_process->read<float>(attack_address);
		}

		return 0.f;
	}

	inline float get_critical_damage()
	{
		if (auto critical_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x788, 0x218 }))
		{
			return g_process->read<float>(critical_address);
		}

		return 0.f;
	}

	inline void attack_multiplier(float attack)
	{
		if (auto attack_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x788, 0x28C }))
		{
			g_process->write(attack_address, attack);
		}
	}

	inline void critical_multiplier(float critical_attack)
	{
		if (auto critical_address = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x788, 0x218 }))
		{
			g_process->write(critical_address, critical_attack);
		}
	}

	inline void freeze_entity(bool activate)
	{
		if (auto fly = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x298, 0x170 }))
		{
			if (activate)
			{
				g_process->write(fly, false);
			}
			else
			{
				g_process->write(fly, true);
			}
		}
	}

	inline void remove_fall_damage(bool activate)
	{
		if (auto fall_damage = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x18A0 }))
		{
			bool is_enable = g_process->read<uint8_t>(fall_damage);

			if (activate && is_enable == 1)
				g_process->write(fall_damage, false);
			else if (!activate && is_enable == 0)
				g_process->write(fall_damage, true);
		}
	}
}