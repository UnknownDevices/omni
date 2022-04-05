#include <omni/app.hpp>

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <omni/logger.hpp> 

#include <omni/imgui/imgui_demo_window.hpp>
#include <omni/imgui/imgui_impl.hpp>
#include <imconfig.h>

namespace Omni
{
    App* App::s_app = nullptr;

    App::App() noexcept = default;

    App::~App() noexcept
    {
        std::destroy_at(&m_platform_window);
    }

    auto App::init(App& app) -> void
    {
        s_app = &app;
		
        ImGui::CreateContext();
		ImGui::StyleColorsLight();

		ImGuiIO& io = ImGui::GetIO(); 
        (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		
        get_app().get_window().get_rendering_layers().push_back(new Imgui_Demo_Window());
    }

    auto App::run() -> void
    {
		glfwMakeContextCurrent(&get_target_platform_window().get_glfw_window());
        
        if (!ImGui_ImplGlfw_InitForOpenGL(&get_target_platform_window().get_glfw_window(), true))
		    Logger::error("Failed to initialize Imgui impl Glfw for OpenGL!");
        if (!ImGui_ImplOpenGL3_Init("#version 410")) 
            Logger::error("Failed to initialize Imgui impl OpenGL3!");
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			Logger::error("Failed to initialize GLAD!");

		glfwMakeContextCurrent(nullptr);
        get_window().start_rendering_thread();

		while (!(bool)glfwWindowShouldClose(&get_target_platform_window().get_glfw_window()))
		{
			glfwPollEvents();
            static uint64 tick_n;
            Logger::spdlog_logger->trace("{0}", tick_n++);
		}
       
		get_window().set_rendering_thread_to_stop();
		get_window().join_rendering_thread();
       
	    ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }
}
