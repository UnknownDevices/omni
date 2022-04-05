#include <omni/imgui/imgui_demo_window.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <omni/app.hpp>

namespace Omni
{
    Imgui_Demo_Window::Imgui_Demo_Window() noexcept
		: m_time(0.0f) {}

	Imgui_Demo_Window::~Imgui_Demo_Window() noexcept = default;

	void Imgui_Demo_Window::on_attach() {}

	void Imgui_Demo_Window::on_detach() {}

	void Imgui_Demo_Window::render()
	{
		ImGuiIO& io = ImGui::GetIO();

 		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);
		
		auto& app = App::get_app();
		io.DisplaySize = ImVec2(app.get_window().get_width(), app.get_window().get_height());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
