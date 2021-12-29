#pragma once

#include "d3d11_include.h"

namespace dxvk {

  /**
   * \brief Common resource description
   * 
   * Stores the usage and bind flags of a resource
   * Can be used to quickly determine whether it is
   * legal to create a view for a given resource.
   */
  struct D3D11_COMMON_RESOURCE_DESC {
    D3D11_RESOURCE_DIMENSION  Dim;
    DXGI_FORMAT               Format;
    D3D11_USAGE               Usage;
    UINT                      BindFlags;
    UINT                      CPUAccessFlags;
    UINT                      MiscFlags;
    UINT                      DxgiUsage;
  };
  

  /**
   * \brief IDXGIResource implementation for D3D11 resources
   */
  class D3D11DXGIResource : public IDXGIResource1 {

  public:
    
    D3D11DXGIResource(
            ID3D11Resource*         pResource);

    ~D3D11DXGIResource();

    ULONG STDMETHODCALLTYPE AddRef();
    
    ULONG STDMETHODCALLTYPE Release();
    
    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                  riid,
            void**                  ppvObject);

    HRESULT STDMETHODCALLTYPE GetPrivateData(
            REFGUID                 Name,
            UINT*                   pDataSize,
            void*                   pData);
    
    HRESULT STDMETHODCALLTYPE SetPrivateData(
            REFGUID                 Name,
            UINT                    DataSize,
      const void*                   pData);
    
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
            REFGUID                 Name,
      const IUnknown*               pUnknown);
    
    HRESULT STDMETHODCALLTYPE GetParent(
            REFIID                  riid,
            void**                  ppParent);
    
    HRESULT STDMETHODCALLTYPE GetDevice(
            REFIID                  riid,
            void**                  ppDevice);

    HRESULT STDMETHODCALLTYPE GetEvictionPriority(
            UINT*                   pEvictionPriority);

    HRESULT STDMETHODCALLTYPE GetSharedHandle(
            HANDLE*                 pSharedHandle);

    HRESULT STDMETHODCALLTYPE GetUsage(
            DXGI_USAGE*             pUsage);

    HRESULT STDMETHODCALLTYPE SetEvictionPriority(
            UINT                    EvictionPriority);

    HRESULT STDMETHODCALLTYPE CreateSharedHandle(
      const SECURITY_ATTRIBUTES*    pAttributes,
            DWORD                   dwAccess,
            LPCWSTR                 lpName,
            HANDLE*                 pHandle);

    HRESULT STDMETHODCALLTYPE CreateSubresourceSurface(
            UINT                    index,
            IDXGISurface2**         ppSurface);

  private:

    ID3D11Resource* m_resource;

  };


  /**
   * \brief Queries common resource description
   * 
   * \param [in] pResource The resource to query
   * \param [out] pDesc Resource description
   * \returns \c S_OK on success, or \c E_INVALIDARG
   */
  HRESULT GetCommonResourceDesc(
          ID3D11Resource*             pResource,
          D3D11_COMMON_RESOURCE_DESC* pDesc);
  
  /**
   * \brief Increments private reference count of a resource
   * 
   * Helper method that figures out the exact type of
   * the resource and calls its \c AddRefPrivate method.
   * \param [in] pResource The resource to reference
   * \returns \c S_OK, or \c E_INVALIDARG for an invalid resource
   */
  HRESULT ResourceAddRefPrivate(
          ID3D11Resource*             pResource);
  
  /**
   * \brief Decrements private reference count of a resource
   * 
   * Helper method that figures out the exact type of
   * the resource and calls its \c ReleasePrivate method.
   * \param [in] pResource The resource to reference
   * \returns \c S_OK, or \c E_INVALIDARG for an invalid resource
   */
  HRESULT ResourceReleasePrivate(
          ID3D11Resource*             pResource);

}