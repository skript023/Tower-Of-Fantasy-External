#pragma once
#include "common.hpp"

namespace ellohim
{
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void on_present();
		void input_handler();
		void draw_overlay();
		void render_on_tick();
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		static DWORD WINAPI process_check(LPVOID lpParameter);
	private:
		void setup_window();
		void init_overlay();
	};

	inline renderer* g_renderer{};
}


