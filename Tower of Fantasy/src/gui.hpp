#pragma once

namespace ellohim
{
	class gui
	{
	public:
		void dx_on_tick();
		void dx_init();

	public:
		bool m_opened{ true };
	};
	inline gui g_gui;
}