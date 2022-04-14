#include <omni/dx12_window.hpp>

#include <omni/resource.h>
#include <omni/omni.hpp>

namespace Omni
{
	LRESULT CALLBACK 
	Dx12_Window::WindowProc(HWND h_wnd, UINT msg_type, WPARAM w_param, LPARAM l_param)
	{
		switch (msg_type) {
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
		}

		return DefWindowProc(h_wnd, msg_type, w_param, l_param);
	}

	void
	Dx12_Window::run()
	{
		using namespace Omni;

		/* - Initialize Global Variables - */

		m_class_name        = "Omni Engine Class";
		m_title             = "Omni Engine";
		m_width             = 780;
		m_height            = 420;

		/* - Create Window Class - */

		WNDCLASSEX wcex;
		wcex.cbSize 		      = sizeof(WNDCLASSEX);
		wcex.style 			      = CS_HREDRAW | CS_VREDRAW;
		wcex.cbClsExtra 		  = 0;
		wcex.cbWndExtra 		  = 0;
		wcex.hCursor 			  = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground        = (HBRUSH)GetStockObject(NULL_BRUSH);
		wcex.hIcon = wcex.hIconSm = (HICON)LoadImage(s_omni_h_inst, MAKEINTRESOURCE(ICON_MAIN),  
			IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
		wcex.lpszClassName 		  = m_class_name;
		wcex.lpszMenuName 		  = nullptr;
		wcex.hInstance 			  = s_omni_h_inst;
		wcex.lpfnWndProc 		  = WindowProc;

		RegisterClassEx(&wcex);

		/* - Create and Display our Window  - */

		HWND h_wnd = CreateWindow(m_class_name, m_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
			m_width, m_height, nullptr, nullptr, s_omni_h_inst, nullptr);
		if (!h_wnd) {
			MessageBox(0, "Failed to Create Window!", 0, 0);
			return;
		}

		ShowWindow(h_wnd, SW_SHOW);

		/* - Listen for Message events - */

		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}
