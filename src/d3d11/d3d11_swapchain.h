#pragma once

#include "d3d11_texture.h"

namespace dxvk {
  
  class D3D11Device;
  class D3D11DXGIDevice;

  class D3D11SwapChain : public ComObject<IDXGIVkSwapChain> {
  public:

    D3D11SwapChain(
            D3D11DXGIDevice*          pContainer,
            D3D11Device*              pDevice,
            HWND                      hWnd,
      const DXGI_SWAP_CHAIN_DESC1*    pDesc);
    
    ~D3D11SwapChain();

    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                    riid,
            void**                    ppvObject);

    HRESULT STDMETHODCALLTYPE GetDesc(
            DXGI_SWAP_CHAIN_DESC1*    pDesc);

    HRESULT STDMETHODCALLTYPE GetAdapter(
            REFIID                    riid,
            void**                    ppvObject);
    
    HRESULT STDMETHODCALLTYPE GetDevice(
            REFIID                    riid,
            void**                    ppDevice);
    
    HRESULT STDMETHODCALLTYPE GetImage(
            UINT                      BufferId,
            REFIID                    riid,
            void**                    ppBuffer);

    UINT STDMETHODCALLTYPE GetImageIndex();

    UINT STDMETHODCALLTYPE GetFrameLatency();

    HANDLE STDMETHODCALLTYPE GetFrameLatencyEvent();

    HRESULT STDMETHODCALLTYPE ChangeProperties(
      const DXGI_SWAP_CHAIN_DESC1*    pDesc);

    HRESULT STDMETHODCALLTYPE SetPresentRegion(
      const RECT*                     pRegion);

    HRESULT STDMETHODCALLTYPE SetGammaControl(
            UINT                      NumControlPoints,
      const DXGI_RGB*                 pControlPoints);

    HRESULT STDMETHODCALLTYPE SetFrameLatency(
            UINT                      MaxLatency);

    HRESULT STDMETHODCALLTYPE Present(
            UINT                      SyncInterval,
            UINT                      PresentFlags,
      const DXGI_PRESENT_PARAMETERS*  pPresentParameters);

    void STDMETHODCALLTYPE NotifyModeChange(
            BOOL                      Windowed,
      const DXGI_MODE_DESC*           pDisplayMode);

  private:

    Com<D3D11DXGIDevice, false> m_dxgiDevice;
    D3D11Device*                m_parent;

    DXGI_SWAP_CHAIN_DESC1     m_desc;
    bool                      m_dirty = true;
    D3D11Texture2D*           m_backBuffer = nullptr;

    void CreateBackBuffer();

  };

}