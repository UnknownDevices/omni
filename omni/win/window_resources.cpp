#include <omni/win/window_resources.hpp>

#include <omni/resource.h>
#include <omni/win/window.hpp>
#include <omni/logger.hpp>

namespace Omni::Win
{
    void WndRscs::increment(std::string& str, size_t begin_i, bool allow_resize)
    {
        auto itr = str.begin() + begin_i;
        while (true)
        {
            if (itr >= str.end())
            {
                if(allow_resize)
                    str.resize(str.length() + 1, 1);
                return;
            }

            if (*itr < 127)
            {
                ++*itr;
                return;
            }

            *itr = 1;
            ++itr;
        }
    }

    void WndRscs::create()
    {
        increment(s_class_name, 5);
        
        WNDCLASSEX wcex;
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_OWNDC;
        wcex.lpfnWndProc   = Wnd::proc_wnd_creation_msgs;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = 0;
        wcex.hInstance     = s_omni_h_inst;
        wcex.hIcon         = static_cast<HICON>(LoadImage(s_omni_h_inst, 
            MAKEINTRESOURCE(ICON_MAIN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
        wcex.hCursor       = nullptr;
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName  = nullptr;
        wcex.lpszClassName = s_class_name.data();
        wcex.hIconSm       = wcex.hIcon;

        m_atom = RegisterClassEx(&wcex);
        omni_assert_win32_call(m_atom, RegisterClassEx);
    }

    void WndRscs::destroy() 
    {
        bool success = UnregisterClass((LPCSTR)m_atom, s_omni_h_inst);
        omni_assert_win32_call(success, UnregisterClass);
    }
}