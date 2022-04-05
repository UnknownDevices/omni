#include <omni/platform/windows/Windows_Window.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <omni/logger.hpp> 

namespace Omni
{
    bool Windows_Window::s_renderer_init = false;
    
	Windows_Window::Windows_Window(bool vsync, int32_fast width, int32_fast height,
        std::string&& title, Callback_Vector&& callbacks, 
        Rendering_Layer_Vector&& renderering_layers) noexcept
        : m_vsync(vsync), m_should_stop_rendering(false), 
        m_height(height), m_width(width),
        m_title(std::move(title)), 
        m_rendering_thread(nullptr),
        m_glfw_window(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr)),
	    m_callbacks(std::move(callbacks)), m_rendering_layers(std::move(renderering_layers)) 
    {
		glfwSetWindowUserPointer(&get_glfw_window(), this);
	    pv_set_glfw_callbacks();
    }

    Windows_Window::~Windows_Window() noexcept
    {
        std::destroy_at(m_rendering_thread);
        glfwDestroyWindow(m_glfw_window);
    }
	
    auto Windows_Window::on_glfw_error(int error, const char* description) -> void
	{
		Logger::error("GLFW Error ({0}): {1}", error, description);
	}

    auto Windows_Window::init_renderer() -> void
	{
		if (is_renderer_init())
			return;

		glfwInit(); 
		glfwSetErrorCallback(Windows_Window::on_glfw_error);
		
		s_renderer_init = true;
	}

	auto Windows_Window::pv_dispatch_event(Window_Event& event) -> void
	{
		for(auto& callback : m_callbacks)
		{
			if(callback(event))
				break;
		}
	}

	auto Windows_Window::start_rendering_thread(bool vsync) -> void
	{
		m_should_stop_rendering = true; // TODO: Invert this
		m_rendering_thread = new std::thread([=]() -> void 
			{ this->pv_render_asynchronous(ImGui::GetCurrentContext(), vsync); });
	}

	auto Windows_Window::set_rendering_thread_to_stop() -> void
	{
		m_should_stop_rendering = false;
	}
	
	auto Windows_Window::join_rendering_thread() -> void
	{
		m_rendering_thread->join();
	}

	auto Windows_Window::pause_rendering() -> void
	{
		m_should_pause_rendering = true;
	}

	auto Windows_Window::resume_rendering() -> void
	{
		m_should_pause_rendering = false;
	}

	auto Windows_Window::pv_set_vsync(bool enable) -> void
	{
		if(enable)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_vsync = enable;
	}

	auto Windows_Window::pv_set_glfw_callbacks() -> void
	{
		// TODO: Find a way to ensure no casts ever ocurr when creating an event;
		glfwSetWindowSizeCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, int width, int height)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));
			window.m_width = width;
			window.m_height = height;

			window.m_latest_window_resize_event = {width, height};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetWindowCloseCallback(&get_glfw_window(), [](GLFWwindow* glfw_window)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_window_close_event = {};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetKeyCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_key_event = {key, scancode, mods, 
				static_cast<Key_Event::Action>(action)};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetMouseButtonCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, int button, int action, int mods)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_mouse_button_event = {button, mods, 
				static_cast<Mouse_Button_Event::Action>(action)};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetCursorEnterCallback(&get_glfw_window(), [](GLFWwindow* glfw_window, int entered)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_mouse_enter_event = {static_cast<bool>(entered)};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetScrollCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, double x_offset, double y_offset)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_mouse_scroll_event = {x_offset, y_offset};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetCursorPosCallback(&get_glfw_window(), 
		[](GLFWwindow* glfw_window, double x_pos, double y_pos)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_mouse_move_event= {x_pos, y_pos};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetCharCallback(&get_glfw_window(), 
		[](GLFWwindow* glfw_window, unsigned int codepoint)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_char_event = {codepoint};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetWindowIconifyCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, int iconified)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_window_iconify_event = {static_cast<bool>(iconified)};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetWindowPosCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, int x_pos, int y_pos)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_window_move_event = {x_pos, y_pos};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetWindowMaximizeCallback(&get_glfw_window(), 
			[](GLFWwindow* glfw_window, int maximised)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_window_maximise_event = {static_cast<bool>(maximised)};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetWindowFocusCallback(&get_glfw_window(), [](GLFWwindow* glfw_window, int focused)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_window_focus_event = {static_cast<bool>(focused)};
			window.pv_dispatch_event(window.get_latest_event());
		});

		glfwSetWindowRefreshCallback(&get_glfw_window(), [](GLFWwindow* glfw_window)
		{
			Windows_Window& window = *static_cast<Windows_Window*>
				(glfwGetWindowUserPointer(glfw_window));

			window.m_latest_window_refresh_event = {};
			window.pv_dispatch_event(window.get_latest_event());
		});

		// TODO: Currently unused glfw callbacks: glfwSetFramebufferSizeCallback, glfwSetWindowContentScaleCallback
	}

	auto Windows_Window::pv_render_asynchronous(ImGuiContext* const imgui_context, 
		bool vsync) -> void
	{
		ImGui::SetCurrentContext(imgui_context);
		glfwMakeContextCurrent(&get_glfw_window());
		pv_set_vsync(m_vsync);

		while(m_should_stop_rendering)
		{
			while(m_should_pause_rendering)
				std::this_thread::yield();

			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

			glClearColor(r, g, b, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for(auto rendering_layer : m_rendering_layers)
			{
				rendering_layer->render();
			}

			glfwSwapBuffers(&get_glfw_window());
		}
	}
}