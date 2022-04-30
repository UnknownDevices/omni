#include <omni/window.hpp>
#include <omni/dx12_window.hpp>

#include <omni/resource.h>
#include <omni/logger.hpp>
#include <omni/core.hpp>

namespace Omni
{
	constexpr
	Dx12Window::Dx12Window() noexcept : m_class_name("Omni Engine Class"), 
		m_title("Omni Engine"), m_width(720), m_height(480) {}

	Dx12Window::~Dx12Window() noexcept = default;

	Dx12Window& 
	Dx12Window::get_app_state(HWND hwnd)
	{
		LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		Dx12Window* p_state = reinterpret_cast<Dx12Window*>(ptr);
		return *p_state;
	}
 
	LRESULT CALLBACK 
	Dx12Window::window_proc(HWND hwnd, UINT msg_type, WPARAM w_param, LPARAM l_param) {
		switch (msg_type) {
			case WM_DESTROY: {
				PostQuitMessage(0);
				return 0;
			}
			case WM_CREATE: {
				CREATESTRUCT* p_create = reinterpret_cast<CREATESTRUCT*>(l_param);
				SetWindowLongPtr(hwnd, GWLP_USERDATA, 
					reinterpret_cast<LONG_PTR>(p_create->lpCreateParams));
			}
			case WM_PAINT: {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				EndPaint(hwnd, &ps);
				return 0;
			}
			default: {
				return DefWindowProc(hwnd, msg_type, w_param, l_param);
			}
		}
	}

	void
	Dx12Window::run() {
		WNDCLASSEX wcex;
		wcex.cbSize 		      = sizeof(WNDCLASSEX);
		wcex.style 			      = CS_HREDRAW | CS_VREDRAW;
		wcex.cbClsExtra 		  = 0;
		wcex.cbWndExtra 		  = 0;
		wcex.hCursor 			  = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground        = static_cast<HBRUSH>(CreateSolidBrush(RGB(40, 44, 52)));
		wcex.hIcon = wcex.hIconSm = static_cast<HICON>(LoadImage(s_omni_h_inst, 
			MAKEINTRESOURCE(ICON_MAIN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
		wcex.lpszClassName 		  = m_class_name;
		wcex.lpszMenuName 		  = nullptr;
		wcex.hInstance 			  = s_omni_h_inst;
		wcex.lpfnWndProc 		  = window_proc;

		RegisterClassEx(&wcex);

		m_hwnd = CreateWindowEx(0, m_class_name, m_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
			CW_USEDEFAULT, 720, 480, nullptr, nullptr, s_omni_h_inst, this);

		if (!m_hwnd) {
			error_log("Failed to Create Window!");
			return;
		}

		ShowWindow(m_hwnd, SW_SHOW);

		MSG msg = {};
		while (true) {
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				if(msg.message == WM_QUIT)
					break;
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}
