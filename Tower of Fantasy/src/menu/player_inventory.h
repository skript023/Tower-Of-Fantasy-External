#pragma once

namespace ellohim
{
	class player_inventory
	{
	public:
		static void render_player_inventory();
	private:
		static inline int credits{};
		static inline int battle_points{};
	};
}