#pragma once
#include "pointer.hpp"

namespace ellohim
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		pointer<uint64_t> m_object;
		pointer<int> m_screen_x;
		pointer<int> m_screen_y;
		pointer<uint64_t> m_uobject;
		pointer<uint64_t> m_world;
		pointer<uint64_t> m_name;
		pointer<uint64_t> m_crouch_speed;
		pointer<uint64_t> m_movement_speed;
		pointer<uint64_t> m_view_matrix;
		pointer<uint8_t> m_cooldown;
		pointer<uint64_t> m_player_nav;
		pointer<uint8_t> m_fast_attack;
		pointer<uint64_t> m_local_player;
	private:
		void disable();
	};

	inline pointers* g_pointers;
}