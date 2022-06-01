#pragma once

const wchar_t c_systemIndexName[] = L"SystemIndex";

inline HRESULT GetCatalogManagerHelper(_In_ PCWSTR catalogName, _Outptr_ ISearchCatalogManager** catalogManager)
{
    winrt::com_ptr<ISearchManager> searchManager;
    RETURN_IF_FAILED(CoCreateInstance(CLSID_CSearchManager, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(searchManager.put())));
    RETURN_IF_FAILED(searchManager->GetCatalog(catalogName, catalogManager));

    return S_OK;
}