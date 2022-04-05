#pragma once

#include <omni/core.h>
#include <omni/rendering_layer.hpp>

namespace Omni
{
	class OMNI_API Imgui_Demo_Window final : public Rendering_Layer
	{
	public:
		Imgui_Demo_Window() noexcept;

		virtual ~Imgui_Demo_Window() noexcept override;

		virtual void render() override;

		virtual void on_attach() override;

		virtual void on_detach() override;

	private:
		float m_time;
	};
}