#include "gui.hpp"
#include "Overlay.h"
#include "Functions.h"
#include "renderer.hpp"
#include "pointers.hpp"
#include "features.hpp"
#include "script.hpp"
#include "utility/utility.hpp"
#include "fonts/font_list.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace ellohim
{
	using namespace overlay;
	using namespace functions;

	void renderer::input_handler()
	{
		for (int i = 0; i < 5; i++) ImGui::GetIO().MouseDown[i] = false;
		int button = -1;
		if (GetAsyncKeyState(VK_LBUTTON)) button = 0;
		if (button != -1) ImGui::GetIO().MouseDown[button] = true;
	}

	void renderer::draw_overlay()
	{
		char FpsInfo[64];
		sprintf(FpsInfo, "Overlay FPS: %0.f", ImGui::GetIO().Framerate);
		RGBA White = { 255.f,255.f,255.f,255.f };
		DrawStrokeText(30.f, 44.f, &White, FpsInfo);

		utility::esp(g_settings->player.esp);
	}

	void renderer::on_present()
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) g_gui.m_opened = !g_gui.m_opened;
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		draw_overlay();
		ImGui::GetIO().MouseDrawCursor = g_gui.m_opened;

		if (g_gui.m_opened == true)
		{
			input_handler();

			g_gui.dx_on_tick();

			SetWindowLong(overlay_window::m_hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
			UpdateWindow(overlay_window::m_hwnd);
			SetFocus(overlay_window::m_hwnd);
		}
		else
		{
			SetWindowLong(overlay_window::m_hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
			UpdateWindow(overlay_window::m_hwnd);
		}
		ImGui::EndFrame();

		d3d9::m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (d3d9::m_d3d_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			d3d9::m_d3d_device->EndScene();
		}

		HRESULT result = d3d9::m_d3d_device->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && d3d9::m_d3d_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3d9::m_d3d_device->Reset(&d3d9::m_d3d_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}

	void renderer::render_on_tick()
	{
		static RECT OldRect;
		ZeroMemory(&d3d9::m_d3d_message, sizeof(MSG));

		if (d3d9::m_d3d_message.message != WM_QUIT)
		{
			if (PeekMessage(&d3d9::m_d3d_message, overlay_window::m_hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&d3d9::m_d3d_message);
				DispatchMessage(&d3d9::m_d3d_message);
			}
			HWND ForegroundWindow = GetForegroundWindow();
			if (ForegroundWindow == window_process::m_hwnd)
			{
				HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
				SetWindowPos(overlay_window::m_hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			RECT TempRect;
			POINT TempPoint;
			ZeroMemory(&TempRect, sizeof(RECT));
			ZeroMemory(&TempPoint, sizeof(POINT));

			GetClientRect(window_process::m_hwnd, &TempRect);
			ClientToScreen(window_process::m_hwnd, &TempPoint);

			TempRect.left = TempPoint.x;
			TempRect.top = TempPoint.y;
			ImGuiIO& io = ImGui::GetIO();
			io.ImeWindowHandle = window_process::m_hwnd;

			if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom)
			{
				OldRect = TempRect;
				window_process::m_screen_resolution.x = TempRect.right;
				window_process::m_screen_resolution.y = TempRect.bottom;
				d3d9::m_d3d_params.BackBufferWidth = window_process::m_screen_resolution.x;
				d3d9::m_d3d_params.BackBufferHeight = window_process::m_screen_resolution.y;
				SetWindowPos(overlay_window::m_hwnd, (HWND)0, TempPoint.x, TempPoint.y, window_process::m_screen_resolution.x, window_process::m_screen_resolution.y, SWP_NOREDRAW);
				d3d9::m_d3d_device->Reset(&d3d9::m_d3d_params);
			}
			TRY_CLAUSE
			{
				on_present();
			} EXCEPT_CLAUSE
		}
	}

	renderer::renderer()
	{
		init_overlay();
		setup_window();

		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9::m_d3d_context)))
		{
			return;
		}

		D3DPRESENT_PARAMETERS Params = { 0 };
		Params.Windowed = TRUE;
		Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		Params.hDeviceWindow = overlay_window::m_hwnd;
		Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
		Params.BackBufferFormat = D3DFMT_A8R8G8B8;
		Params.BackBufferWidth = window_process::m_screen_resolution.x;
		Params.BackBufferHeight = window_process::m_screen_resolution.y;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		Params.EnableAutoDepthStencil = TRUE;
		Params.AutoDepthStencilFormat = D3DFMT_D16;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

		if (FAILED(d3d9::m_d3d_context->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlay_window::m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &d3d9::m_d3d_device)))
		{
			d3d9::m_d3d_context->Release();
			return;
		}

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui_ImplWin32_Init(overlay_window::m_hwnd);
		ImGui_ImplDX9_Init(d3d9::m_d3d_device);

		ImFontConfig font_cfg{};
		font_cfg.FontDataOwnedByAtlas = false;
		std::strcpy(font_cfg.Name, "Rubik");

		ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 14.f, &font_cfg, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

		ImGui::GetIO().Fonts->AddFontDefault();

		d3d9::m_d3d_context->Release();

		g_renderer = this;
	}

	renderer::~renderer()
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		if (d3d9::m_d3d_device != NULL)
		{
			d3d9::m_d3d_device->EndScene();
			d3d9::m_d3d_device->Release();
		}
		if (d3d9::m_d3d_context != NULL)
		{
			d3d9::m_d3d_context->Release();
		}
		DestroyWindow(overlay_window::m_hwnd);
		UnregisterClass(overlay_window::m_window_class.lpszClassName, overlay_window::m_window_class.hInstance);

		g_renderer = nullptr;
	}

	LRESULT CALLBACK renderer::WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
			return true;

		switch (Message)
		{
		case WM_DESTROY:
			if (d3d9::m_d3d_device != NULL)
			{
				d3d9::m_d3d_device->EndScene();
				d3d9::m_d3d_device->Release();
			}
			if (d3d9::m_d3d_context != NULL)
			{
				d3d9::m_d3d_context->Release();
			}
			PostQuitMessage(0);
			exit(4);
			break;
		case WM_SIZE:
			if (d3d9::m_d3d_device != NULL && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX9_InvalidateDeviceObjects();
				d3d9::m_d3d_params.BackBufferWidth = LOWORD(lParam);
				d3d9::m_d3d_params.BackBufferHeight = HIWORD(lParam);
				HRESULT hr = d3d9::m_d3d_device->Reset(&d3d9::m_d3d_params);
				if (hr == D3DERR_INVALIDCALL)
					IM_ASSERT(0);
				ImGui_ImplDX9_CreateDeviceObjects();
			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
			break;
		}
		return 0;
	}

	void renderer::setup_window()
	{
		overlay_window::m_window_class = {
			sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, overlay_window::m_name, LoadIcon(nullptr, IDI_APPLICATION)
		};

		RegisterClassEx(&overlay_window::m_window_class);
		if (window_process::m_hwnd)
		{
			static RECT TempRect = { NULL };
			static POINT TempPoint;
			GetClientRect(window_process::m_hwnd, &TempRect);
			ClientToScreen(window_process::m_hwnd, &TempPoint);
			TempRect.left = TempPoint.x;
			TempRect.top = TempPoint.y;
			window_process::m_screen_resolution.x = TempRect.right;
			window_process::m_screen_resolution.y = TempRect.bottom;
		}

		overlay_window::m_hwnd = CreateWindowEx(NULL, overlay_window::m_name, overlay_window::m_name, WS_POPUP | WS_VISIBLE, window_process::m_screen_pos.m_window_left, window_process::m_screen_pos.m_window_top, window_process::m_screen_resolution.x, window_process::m_screen_resolution.y, NULL, NULL, 0, NULL);
		DwmExtendFrameIntoClientArea(overlay_window::m_hwnd, &d3d9::m_d3d_margin);
		SetWindowLong(overlay_window::m_hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
		ShowWindow(overlay_window::m_hwnd, SW_SHOW);
		UpdateWindow(overlay_window::m_hwnd);
	}

	DWORD WINAPI renderer::process_check(LPVOID lpParameter)
	{
		if (window_process::m_hwnd != NULL)
		{
			if (GetProcessId(m_target_process) == 0)
			{
				exit(0);
			}
		}
	}

	void renderer::init_overlay()
	{
		if (m_create_console == false)
			ShowWindow(GetConsoleWindow(), SW_HIDE);

		bool WindowFocus = false;
		while (WindowFocus == false)
		{
			DWORD ForegroundWindowProcessID;
			GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
			if (functions::GetProcessId(m_target_process) == ForegroundWindowProcessID)
			{
				window_process::m_id = GetCurrentProcessId();
				window_process::m_handle = GetCurrentProcess();
				window_process::m_hwnd = GetForegroundWindow();

				RECT TempRect;
				GetWindowRect(window_process::m_hwnd, &TempRect);
				window_process::m_screen_resolution.x = TempRect.right - TempRect.left;
				window_process::m_screen_resolution.y = TempRect.bottom - TempRect.top;
				window_process::m_screen_pos.m_window_left = TempRect.left;
				window_process::m_screen_pos.m_window_right = TempRect.right;
				window_process::m_screen_pos.m_window_top = TempRect.top;
				window_process::m_screen_pos.m_window_botton = TempRect.bottom;

				char TempTitle[MAX_PATH];
				GetWindowText(window_process::m_hwnd, TempTitle, sizeof(TempTitle));
				window_process::m_title = TempTitle;

				char TempClassName[MAX_PATH];
				GetClassName(window_process::m_hwnd, TempClassName, sizeof(TempClassName));
				window_process::m_class_name = TempClassName;

				char TempPath[MAX_PATH];
				GetModuleFileNameEx(window_process::m_handle, NULL, TempPath, sizeof(TempPath));
				window_process::m_path = TempPath;

				WindowFocus = true;
			}
		}
	}
}
