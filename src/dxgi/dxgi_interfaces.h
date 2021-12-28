#pragma once

#include "../dxvk/dxvk_include.h"

#include "dxgi_format.h"
#include "dxgi_include.h"

namespace dxvk {
  class DxgiAdapter;
  class DxgiSwapChain;
  class DxvkAdapter;
  class DxvkBuffer;
  class DxvkDevice;
  class DxvkImage;
}


/**
 * \brief Per-monitor data
 */
struct DXGI_VK_MONITOR_DATA {
  dxvk::DxgiSwapChain*  pSwapChain;
  DXGI_FRAME_STATISTICS FrameStats;
  DXGI_GAMMA_CONTROL    GammaCurve;
};


/**
 * \brief Private DXGI presenter
 * 
 * Presenter interface that allows the DXGI swap
 * chain implementation to remain API-agnostic,
 * so that common code can stay in one class.
 */
MIDL_INTERFACE("104001a6-7f36-4957-b932-86ade9567d91")
IDXGIVkSwapChain : public IUnknown {
  virtual HRESULT STDMETHODCALLTYPE GetDesc(
          DXGI_SWAP_CHAIN_DESC1*    pDesc) = 0;
  
  virtual HRESULT STDMETHODCALLTYPE GetAdapter(
          REFIID                    riid,
          void**                    ppvObject) = 0;
  
  virtual HRESULT STDMETHODCALLTYPE GetDevice(
          REFIID                    riid,
          void**                    ppDevice) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetImage(
          UINT                      BufferId,
          REFIID                    riid,
          void**                    ppBuffer) = 0;

  virtual UINT STDMETHODCALLTYPE GetImageIndex() = 0;

  virtual UINT STDMETHODCALLTYPE GetFrameLatency() = 0;

  virtual HANDLE STDMETHODCALLTYPE GetFrameLatencyEvent() = 0;

  virtual HRESULT STDMETHODCALLTYPE ChangeProperties(
    const DXGI_SWAP_CHAIN_DESC1*    pDesc) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetPresentRegion(
    const RECT*                     pRegion) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetGammaControl(
          UINT                      NumControlPoints,
    const DXGI_RGB*                 pControlPoints) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetFrameLatency(
          UINT                      MaxLatency) = 0;

  virtual HRESULT STDMETHODCALLTYPE Present(
          UINT                      SyncInterval,
          UINT                      PresentFlags,
    const DXGI_PRESENT_PARAMETERS*  pPresentParameters) = 0;

  virtual void STDMETHODCALLTYPE NotifyModeChange(
          BOOL                      Windowed,
    const DXGI_MODE_DESC*           pDisplayMode) = 0;
};


/**
 * \brief Private DXGI adapter interface
 * 
 * The implementation of \c IDXGIAdapter holds a
 * \ref DxvkAdapter which can be retrieved using
 * this interface.
 */
MIDL_INTERFACE("907bf281-ea3c-43b4-a8e4-9f231107b4ff")
IDXGIDXVKAdapter : public IDXGIAdapter4 {
  virtual dxvk::Rc<dxvk::DxvkAdapter> STDMETHODCALLTYPE GetDXVKAdapter() = 0;

  virtual dxvk::Rc<dxvk::DxvkInstance> STDMETHODCALLTYPE GetDXVKInstance() = 0;

};


/**
 * \brief Private DXGI device interface
 */
MIDL_INTERFACE("92a5d77b-b6e1-420a-b260-fdd701272827")
IDXGIDXVKDevice : public IUnknown {
  virtual void STDMETHODCALLTYPE SetAPIVersion(
            UINT                    Version) = 0;

  virtual UINT STDMETHODCALLTYPE GetAPIVersion() = 0;

};


/**
 * \brief Private DXGI monitor info interface
 * 
 * Can be queried from the DXGI factory to store monitor
 * info globally, with a lifetime that exceeds that of
 * the \c IDXGIOutput or \c IDXGIAdapter objects.
 */
MIDL_INTERFACE("c06a236f-5be3-448a-8943-89c611c0c2c1")
IDXGIVkMonitorInfo : public IUnknown {
  /**
   * \brief Initializes monitor data
   * 
   * Fails if data for the given monitor already exists.
   * \param [in] hMonitor The monitor handle
   * \param [in] pData Initial data
   */
  virtual HRESULT STDMETHODCALLTYPE InitMonitorData(
          HMONITOR                hMonitor,
    const DXGI_VK_MONITOR_DATA*   pData) = 0;

  /**
   * \brief Retrieves and locks monitor data
   * 
   * Fails if no data for the given monitor exists.
   * \param [in] hMonitor The monitor handle
   * \param [out] Pointer to monitor data
   * \returns S_OK on success
   */
  virtual HRESULT STDMETHODCALLTYPE AcquireMonitorData(
          HMONITOR                hMonitor,
          DXGI_VK_MONITOR_DATA**  ppData) = 0;
  
  /**
   * \brief Unlocks monitor data
   * 
   * Must be called after each successful
   * call to \ref AcquireMonitorData.
   * \param [in] hMonitor The monitor handle
   */
  virtual void STDMETHODCALLTYPE ReleaseMonitorData() = 0;

};


struct D3D11_TEXTURE2D_DESC1;
struct ID3D11Texture2D;


/**
 * \brief IWineDXGISwapChainFactory device interface
 *
 * Allows a swap chain to be created from a device.
 * See include/wine/winedxgi.idl for definition.
 */
MIDL_INTERFACE("53cb4ff0-c25a-4164-a891-0e83db0a7aac")
IWineDXGISwapChainFactory : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE CreateSwapChainForHwnd(
            IDXGIFactory*           pFactory,
            HWND                    hWnd,
      const DXGI_SWAP_CHAIN_DESC1*  pDesc,
      const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
            IDXGIOutput*            pRestrictToOutput,
            IDXGISwapChain1**       ppSwapChain) = 0;
};


#ifdef _MSC_VER
struct __declspec(uuid("907bf281-ea3c-43b4-a8e4-9f231107b4ff")) IDXGIDXVKAdapter;
struct __declspec(uuid("92a5d77b-b6e1-420a-b260-fdd701272827")) IDXGIDXVKDevice;
struct __declspec(uuid("c06a236f-5be3-448a-8943-89c611c0c2c1")) IDXGIVkMonitorInfo;
struct __declspec(uuid("104001a6-7f36-4957-b932-86ade9567d91")) IDXGIVkSwapChain;
struct __declspec(uuid("53cb4ff0-c25a-4164-a891-0e83db0a7aac")) IWineDXGISwapChainFactory;
#else
__CRT_UUID_DECL(IDXGIDXVKAdapter,          0x907bf281,0xea3c,0x43b4,0xa8,0xe4,0x9f,0x23,0x11,0x07,0xb4,0xff);
__CRT_UUID_DECL(IDXGIDXVKDevice,           0x92a5d77b,0xb6e1,0x420a,0xb2,0x60,0xfd,0xf7,0x01,0x27,0x28,0x27);
__CRT_UUID_DECL(IDXGIVkMonitorInfo,        0xc06a236f,0x5be3,0x448a,0x89,0x43,0x89,0xc6,0x11,0xc0,0xc2,0xc1);
__CRT_UUID_DECL(IDXGIVkSwapChain,          0x104001a6,0x7f36,0x4957,0xb9,0x32,0x86,0xad,0xe9,0x56,0x7d,0x91);
__CRT_UUID_DECL(IWineDXGISwapChainFactory, 0x53cb4ff0,0xc25a,0x4164,0xa8,0x91,0x0e,0x83,0xdb,0x0a,0x7a,0xac);
#endif
