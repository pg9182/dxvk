#include <cstdlib>
#include <cstring>

#include <d3d10_1.h>

#include "dxgi_adapter.h"
#include "dxgi_enums.h"
#include "dxgi_factory.h"
#include "dxgi_format.h"
#include "dxgi_options.h"
#include "dxgi_output.h"

#include "../util/util_luid.h"

namespace dxvk {

  DxgiAdapter::DxgiAdapter(
          DxgiFactory*      factory,
    const Rc<DxvkAdapter>&  adapter,
          UINT              index)
  : m_factory (factory),
    m_adapter (adapter),
    m_index   (index) {
    
  }
  
  
  DxgiAdapter::~DxgiAdapter() {
    if (m_eventThread.joinable()) {
      std::unique_lock<dxvk::mutex> lock(m_mutex);
      m_eventCookie = ~0u;
      m_cond.notify_one();

      lock.unlock();
      m_eventThread.join();
    }
  }


  ULONG STDMETHODCALLTYPE DxgiAdapter::AddRef() {
    return m_factory->AddRef();
  }
  

  ULONG STDMETHODCALLTYPE DxgiAdapter::Release() {
    return m_factory->Release();
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::QueryInterface(REFIID riid, void** ppvObject) {
    if (ppvObject == nullptr)
      return E_POINTER;

    *ppvObject = nullptr;
    
    if (riid == __uuidof(IUnknown)
     || riid == __uuidof(IDXGIObject)
     || riid == __uuidof(IDXGIAdapter)
     || riid == __uuidof(IDXGIAdapter1)
     || riid == __uuidof(IDXGIAdapter2)
     || riid == __uuidof(IDXGIAdapter3)
     || riid == __uuidof(IDXGIAdapter4)
     || riid == __uuidof(IDXGIDXVKAdapter)) {
      *ppvObject = ref(this);
      return S_OK;
    }
    
    Logger::warn("DxgiAdapter::QueryInterface: Unknown interface query");
    Logger::warn(str::format(riid));
    return E_NOINTERFACE;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetParent(REFIID riid, void** ppParent) {
    return m_factory->QueryInterface(riid, ppParent);
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::CheckInterfaceSupport(
          REFGUID                   InterfaceName,
          LARGE_INTEGER*            pUMDVersion) {
    HRESULT hr = DXGI_ERROR_UNSUPPORTED;

    if (InterfaceName == __uuidof(IDXGIDevice)
     || InterfaceName == __uuidof(ID3D10Device)
     || InterfaceName == __uuidof(ID3D10Device1))
      hr = S_OK;

    // We can't really reconstruct the version numbers
    // returned by Windows drivers from Vulkan data
    if (SUCCEEDED(hr) && pUMDVersion)
      pUMDVersion->QuadPart = ~0ull;

    if (FAILED(hr)) {
      Logger::err("DXGI: CheckInterfaceSupport: Unsupported interface");
      Logger::err(str::format(InterfaceName));
    }

    return hr;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::EnumOutputs(
          UINT                      Output,
          IDXGIOutput**             ppOutput) {
    InitReturnPtr(ppOutput);
    
    if (ppOutput == nullptr)
      return E_INVALIDARG;
    
    if (Output != 0)
      return DXGI_ERROR_NOT_FOUND;
    
    *ppOutput = ref(new DxgiOutput(m_factory, this));
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetDesc(DXGI_ADAPTER_DESC* pDesc) {
    if (pDesc == nullptr)
      return E_INVALIDARG;

    Logger::info("returned fake adapter desc");
    
    *pDesc = (DXGI_ADAPTER_DESC){
      .VendorId              = 0x10DE,
      .DeviceId              = 0x0A27,
      .SubSysId              = 0,
      .Revision              = 0,
      .DedicatedVideoMemory  = 1073741824, // 1 GB
      .DedicatedSystemMemory = 0,
      .SharedSystemMemory    = 0,
      .AdapterLuid           = (LUID){0, 0},
    };
    
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetDesc1(DXGI_ADAPTER_DESC1* pDesc) {
    if (pDesc == nullptr)
      return E_INVALIDARG;
    
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetDesc2(DXGI_ADAPTER_DESC2* pDesc) {
    if (pDesc == nullptr)
      return E_INVALIDARG;
    
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetDesc3(
          DXGI_ADAPTER_DESC3*       pDesc) {
    if (pDesc == nullptr)
      return E_INVALIDARG;
    
    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiAdapter::QueryVideoMemoryInfo(
          UINT                          NodeIndex,
          DXGI_MEMORY_SEGMENT_GROUP     MemorySegmentGroup,
          DXGI_QUERY_VIDEO_MEMORY_INFO* pVideoMemoryInfo) {
    if (NodeIndex > 0 || !pVideoMemoryInfo)
      return E_INVALIDARG;
    
    if (MemorySegmentGroup != DXGI_MEMORY_SEGMENT_GROUP_LOCAL
     && MemorySegmentGroup != DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL)
      return E_INVALIDARG;
    
    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiAdapter::SetVideoMemoryReservation(
          UINT                          NodeIndex,
          DXGI_MEMORY_SEGMENT_GROUP     MemorySegmentGroup,
          UINT64                        Reservation) {    
    Logger::stub(__func__);
    return E_NOTIMPL;
  }

  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::RegisterHardwareContentProtectionTeardownStatusEvent(
          HANDLE                        hEvent,
          DWORD*                        pdwCookie) {
    Logger::stub(__func__);
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiAdapter::RegisterVideoMemoryBudgetChangeNotificationEvent(
          HANDLE                        hEvent,
          DWORD*                        pdwCookie) {
    if (!hEvent || !pdwCookie)
      return E_INVALIDARG;
    Logger::stub(__func__);
    return E_NOTIMPL;
  }
  

  void STDMETHODCALLTYPE DxgiAdapter::UnregisterHardwareContentProtectionTeardownStatus(
          DWORD                         dwCookie) {
    Logger::stub(__func__);
  }


  void STDMETHODCALLTYPE DxgiAdapter::UnregisterVideoMemoryBudgetChangeNotification(
          DWORD                         dwCookie) {
    std::unique_lock<dxvk::mutex> lock(m_mutex);
    m_eventMap.erase(dwCookie);
  }


  Rc<DxvkAdapter> STDMETHODCALLTYPE DxgiAdapter::GetDXVKAdapter() {
    return m_adapter;
  }


  Rc<DxvkInstance> STDMETHODCALLTYPE DxgiAdapter::GetDXVKInstance() {
    return m_factory->GetDXVKInstance();
  }
  
}
