#pragma once

#include <omni/win/incl_windows.h>
#include <d3d11.h>

#include <omni/win/window.hpp>
#include <omni/core.hpp>

namespace Omni
{
class Graphics
{
public:
    OMNI_CONSTEXPR Graphics() noexcept = default;
    OMNI_CONSTEXPR ~Graphics() noexcept = default;

    void make(const Window& wnd);
    void destroy();

	void start_frame() const;
	void end_frame() const;

    void clear_buffer(float r, float g, float b, float o = 1.0f) const;

    ID3D11Device& device() const
    {
        return *device_;
    }

    IDXGISwapChain& swap_chain() const
    {
        return *swap_chain_;
    }

    ID3D11DeviceContext& context() const
    {
        return *context_;
    }

    ID3D11RenderTargetView& target_view() const
    {
        return *target_view_;
    }

private:
    ID3D11Device*           device_;
    IDXGISwapChain*         swap_chain_;
    ID3D11DeviceContext*    context_;
    ID3D11RenderTargetView* target_view_;
};
}
