#pragma once
#include "utility/utility.hpp"

namespace ellohim::attack_stats
{
	inline int get_weapon_type()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<int>(skill_component + 0x1600);
		}

		return 0;
	}

	inline float get_pysical_attack()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<float>(skill_component + 0x160C);
		}

		return 0.f;
	}

	inline void pysical_attack(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x160C, amount);
		}
	}

	inline float fire_attack()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<float>(skill_component + 0x1610);
		}

		return 0.f;
	}

	inline void fire_attack(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1610, amount);
		}
	}

	inline float thunder_attack()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->read<float>(skill_component + 0x1614);
		}

		return 0.f;
	}

	inline void thunder_attack(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1614, amount);
		}
	}

	inline void ice_attack(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1618, amount);
		}
	}

	inline void super_power_attack(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x161C, amount);
		}
	}

	inline void critical(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1634, amount);
		}
	}

	inline float get_critical_damage()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<float>(skill_component + 0x1668);
		}
		return 0.f;
	}

	inline float critical_damage()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<float>(skill_component + 0x1668);
		}
		return 0.f;
	}

	inline void critical_damage(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1668, amount);
		}
	}

	inline float get_critical_rate()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<float>(skill_component + 0x16B0);
		}
		return 0.f;
	}

	inline void critical_rate(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x16B0, amount);
		}
	}

	inline void rapid_attack(bool activate)
	{
		if (activate)
		{
			int weapon = get_weapon_type();
			switch (weapon)
			{
			case 2:
				g_process->write(g_pointers->m_fast_attack, '\xC0');
				break;
			case 3:
				g_process->write(g_pointers->m_fast_attack, '\xC3');
				break;
			}
		}
		else if (!activate)
		{
			g_process->write(g_pointers->m_fast_attack, '\xC2');
		}
	}

	inline void remove_cooldown(bool activate)
	{
		if (*g_pointers->m_cooldown)
		{
			if (activate && *g_pointers->m_cooldown == 0x74)
			{
				g_process->write(g_pointers->m_cooldown, '\x75');
			}
			else if (!activate && *g_pointers->m_cooldown == 0x75)
			{
				g_process->write(g_pointers->m_cooldown, '\x74');
			}
		}
	}
}