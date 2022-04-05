#pragma once 

#include <omni/core.h>

namespace Omni
{
	class Rendering_Layer
	{
	public:
		virtual ~Rendering_Layer() noexcept = default;

		virtual void render() = 0;

		virtual void on_attach() = 0;
		
		virtual void on_detach() = 0;

	protected:
		constexpr Rendering_Layer() noexcept {}
	};
}