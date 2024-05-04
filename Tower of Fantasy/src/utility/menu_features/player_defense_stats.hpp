#pragma once
#include "utility/utility.hpp"

namespace ellohim::defense_stat
{
	inline void max_health(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1604, amount);
		}
	}

	inline void max_energy(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1608, amount);
		}
	}

	inline void energy_recover_multiplier(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x166C, amount);
		}
	}

	inline float get_health()
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			return g_process->read<float>(skill_component + 0x1648);
		}
		return 0.f;
	}

	inline void health(float health)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1648, health);
		}
	}

	inline void pysical_defense(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1620, amount);
		}
	}

	inline void fire_defense(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1624, amount);
		}
	}

	inline void thunder_defense(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1628, amount);
		}
	}

	inline void ice_defense(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x162C, amount);
		}
	}

	inline void super_power_defense(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1630, amount);
		}
	}

	inline void crit_defense(float amount)
	{
		if (auto skill_component = memory::readPointer(g_pointers->m_world, { 0x188, 0x38, 0x0, 0x30, 0x260, 0x780, 0x0 }))
		{
			g_process->write(skill_component + 0x1638, amount);
		}
	}
}