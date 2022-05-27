#include <omni/win/graphics.hpp>
#include <comdef.h>

namespace Omni
{
void Graphics::make(const Window& wnd)
{
    auto sd = DXGI_SWAP_CHAIN_DESC();
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2;
    sd.OutputWindow = wnd.get_hwnd();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL ;
    sd.Flags = 0;

    auto hres = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swap_chain_,
        &device_,
        nullptr,
        &context_);
    omni_assert_win32_call(SUCCEEDED(hres), D3D11CreateDeviceAndSwapChain);

    ID3D11Resource* back_buffer;
    hres = get_swap_chain().GetBuffer(0, __uuidof(ID3D11Resource),
        reinterpret_cast<void**>(&back_buffer));
    omni_assert_win32_call(SUCCEEDED(hres), IDXGISwapChain::GetBuffer);

    hres = get_device().CreateRenderTargetView(
        back_buffer,
        nullptr,
        &target_view_);
    omni_assert_win32_call(SUCCEEDED(hres), ID3D11Device::CreateRenderTargetView);

    back_buffer->Release();
}

void Graphics::destroy()
{
    get_target_view().Release();
    get_context().Release();
    get_swap_chain().Release();
    get_device().Release();
}


void Graphics::start_frame() const
{}

void Graphics::end_frame() const
{
    auto hres = get_swap_chain().Present(1, 0);
    omni_assert_win32_call(SUCCEEDED(hres), IDXGISwapChain::Present);
}

void Graphics::clear_buffer(float r, float g, float b, float o) const
{
    const float color[4] ={r, g, b, o};
    get_context().ClearRenderTargetView(target_view_, color);
}
}
