#include "pointers.hpp"
#include "memory/pattern.hpp"

namespace ellohim
{
	pointers::pointers():
		m_world(pattern("GWorld", "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41 B0 01").add(3).rip()),//48 0F 45 05 ? ? ? ? 80 3D ? ? ? ? ? 72 C3 48 85 C0
		m_object(pattern("GObject", "48 8B 05 ? ? ? ? C1 F9 10 48 63 C9 48 8B 14 C8 4B 8D 0C 40 4C 8D 04 CA").add(3).rip()), //Alt :  48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8D 8D
		m_name(pattern("GName", "48 8D 05 ? ? ? ? EB 13 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? ? 0F 10 03 4C 8D 44 24").add(3).rip()),
		m_screen_x(pattern("Resolution X", "8B 0D ? ? ? ? 8B 05 ? ? ? ? 41 89 4E 04 E9 ? ? ? ? 32 C9").add(8).rip()),
		m_screen_y(pattern("Resolution Y", "8B 0D ? ? ? ? 8B 05 ? ? ? ? 41 89 4E 04 E9 ? ? ? ? 32 C9").add(2).rip()),
		m_view_matrix(pattern("Matrix", "48 8B 05 ? ? ? ? 48 85 C0 74 0B 48 89 58 08 48 8B 05 ? ? ? ? 48 89 03").add(3).rip()),
		m_movement_speed(pattern("Movement", "48 89 3D ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 89 05 ? ? ? ? 40 88 3D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9").add(3).rip()),
		m_crouch_speed(pattern("Crouch", "48 89 05 ? ? ? ? E8 ? ? ? ? 48 8B 45 38 48 89 05 ? ? ? ? 48 85 DB 74 08 48 8B CB").add(3).rip()),
		m_uobject(pattern("GUObject", "E8 ? ? ? ? 48 8B 74 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 83 C4 20 5F C3 40 57").add(1).rip()),
		m_cooldown(pattern("Cooldown", "74 ? 83 79 08 ? 7F")),
		m_player_nav(pattern("Player Navigation", "48 89 05 ? ? ? ? E8 ? ? ? ? 48 8B 45 38 48 89 05 ? ? ? ? 48 85 DB").add(3).rip()),
		m_fast_attack(pattern("Fast Attack", "0F 28 C2 74 12 F3 0F 10 4F ? F3 0F 5C 4C 24 ? 0F 2F D1").add(2)),//45 0f 10 65 ? 4c 8d 45 ? 49 8b d5 4c 89 75 ? 48 8d 4f ? e8 ? ? ? ? 44 0f b6 f8
		m_local_player(pattern("Local Player", "48 89 3D ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 89 05 ? ? ? ? 40 88 3D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9").add(3).rip())
	{	
		auto m_hwnd = FindWindowA("UnrealWindow", "Tower of Fantasy  ");

		if (!m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		disable();
		g_pointers = nullptr;
	}

	void pointers::disable()
	{
		g_process->write(m_cooldown, '\x74');
		g_process->write(g_pointers->m_fast_attack, '\xC2');
	}
}