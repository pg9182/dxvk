#include <algorithm>
#include <numeric>

#include <cstdlib>
#include <cstring>

#include <sstream>
#include <string>

#include "dxgi_adapter.h"
#include "dxgi_factory.h"
#include "dxgi_output.h"
#include "dxgi_swapchain.h"

#include "../dxvk/dxvk_format.h"

namespace dxvk {
  
  DxgiOutput::DxgiOutput(
    const Com<DxgiFactory>& factory,
    const Com<DxgiAdapter>& adapter)
  : m_monitorInfo(factory->GetMonitorInfo()),
    m_adapter(adapter) { 
  }
  
  
  DxgiOutput::~DxgiOutput() {
    
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::QueryInterface(REFIID riid, void** ppvObject) {
    if (ppvObject == nullptr)
      return E_POINTER;

    *ppvObject = nullptr;
    
    if (riid == __uuidof(IUnknown)
     || riid == __uuidof(IDXGIObject)
     || riid == __uuidof(IDXGIOutput)
     || riid == __uuidof(IDXGIOutput1)
     || riid == __uuidof(IDXGIOutput2)
     || riid == __uuidof(IDXGIOutput3)
     || riid == __uuidof(IDXGIOutput4)
     || riid == __uuidof(IDXGIOutput5)
     || riid == __uuidof(IDXGIOutput6)) {
      *ppvObject = ref(this);
      return S_OK;
    }
    
    Logger::warn("DxgiOutput::QueryInterface: Unknown interface query");
    Logger::warn(str::format(riid));
    return E_NOINTERFACE;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::GetParent(REFIID riid, void **ppParent) {
    return m_adapter->QueryInterface(riid, ppParent);
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::FindClosestMatchingMode(
    const DXGI_MODE_DESC *pModeToMatch,
          DXGI_MODE_DESC *pClosestMatch,
          IUnknown       *pConcernedDevice) {
    if (!pModeToMatch || !pClosestMatch)
      return DXGI_ERROR_INVALID_CALL;

    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::FindClosestMatchingMode1(
    const DXGI_MODE_DESC1*      pModeToMatch,
          DXGI_MODE_DESC1*      pClosestMatch,
          IUnknown*             pConcernedDevice) {
    if (!pModeToMatch || !pClosestMatch)
      return DXGI_ERROR_INVALID_CALL;

    if (pModeToMatch->Format == DXGI_FORMAT_UNKNOWN && !pConcernedDevice)
      return DXGI_ERROR_INVALID_CALL;

    // Both or neither must be zero
    if ((pModeToMatch->Width == 0) ^ (pModeToMatch->Height == 0))
      return DXGI_ERROR_INVALID_CALL;

    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::GetDesc(DXGI_OUTPUT_DESC *pDesc) {
    if (pDesc == nullptr)
      return DXGI_ERROR_INVALID_CALL;

    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::GetDesc1(
          DXGI_OUTPUT_DESC1*    pDesc) {
    if (pDesc == nullptr)
      return DXGI_ERROR_INVALID_CALL;

    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::GetDisplayModeList(
          DXGI_FORMAT    EnumFormat,
          UINT           Flags,
          UINT*          pNumModes,
          DXGI_MODE_DESC* pDesc) {
    if (pNumModes == nullptr)
      return DXGI_ERROR_INVALID_CALL;

    if (*pNumModes < 1)
      return DXGI_ERROR_MORE_DATA;

    Logger::info("returned fake display mode");

    if (pDesc) {
      *pDesc = (DXGI_MODE_DESC){
          .Width       = 1280,
          .Height      = 720,
          .RefreshRate = (DXGI_RATIONAL){
              .Numerator   = 60,
              .Denominator = 1,
          },
          .Format           = EnumFormat,
          .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
          .Scaling          = DXGI_MODE_SCALING_UNSPECIFIED,
      };
    }
    *pNumModes = 1;
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::GetDisplayModeList1(
          DXGI_FORMAT           EnumFormat,
          UINT                  Flags,
          UINT*                 pNumModes,
          DXGI_MODE_DESC1*      pDesc) {
    if (pNumModes == nullptr)
      return DXGI_ERROR_INVALID_CALL;

    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::GetDisplaySurfaceData(IDXGISurface* pDestination) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::GetGammaControl(DXGI_GAMMA_CONTROL* pArray) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::GetGammaControlCapabilities(DXGI_GAMMA_CONTROL_CAPABILITIES* pGammaCaps) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  void STDMETHODCALLTYPE DxgiOutput::ReleaseOwnership() {
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::SetDisplaySurface(IDXGISurface* pScanoutSurface) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::GetDisplaySurfaceData1(IDXGIResource* pDestination) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::SetGammaControl(const DXGI_GAMMA_CONTROL* pArray) {
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::TakeOwnership(
          IUnknown *pDevice,
          BOOL     Exclusive) {
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::WaitForVBlank() {
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiOutput::DuplicateOutput(
          IUnknown*                 pDevice,
          IDXGIOutputDuplication**  ppOutputDuplication) {
    if (!pDevice)
      return E_INVALIDARG;
    return S_OK;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::DuplicateOutput1(
          IUnknown*                 pDevice,
          UINT                      Flags,
          UINT                      SupportedFormatsCount,
    const DXGI_FORMAT*              pSupportedFormats,
          IDXGIOutputDuplication**  ppOutputDuplication) {
    InitReturnPtr(ppOutputDuplication);

    if (!pDevice)
      return E_INVALIDARG;
    return S_OK;
  }


  BOOL DxgiOutput::SupportsOverlays() {
    return FALSE;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::CheckOverlaySupport(
          DXGI_FORMAT EnumFormat,
          IUnknown*   pConcernedDevice,
          UINT*       pFlags) {
    Logger::stub(__func__);
    return DXGI_ERROR_UNSUPPORTED;
  }


  HRESULT STDMETHODCALLTYPE DxgiOutput::CheckOverlayColorSpaceSupport(
          DXGI_FORMAT           Format,
          DXGI_COLOR_SPACE_TYPE ColorSpace,
          IUnknown*             pConcernedDevice,
          UINT*                 pFlags) {
    return DXGI_ERROR_UNSUPPORTED;
  }
  

  HRESULT STDMETHODCALLTYPE DxgiOutput::CheckHardwareCompositionSupport(
          UINT*                 pFlags) {
    *pFlags = 0;
    return S_OK;
  }

}
