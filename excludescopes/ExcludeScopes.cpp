// ExcludeIDEProjects.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <SearchAPI.h>
#include <wrl\client.h>
#include <wil\result.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "SearchSDK.lib")

int wmain(int argc, wchar_t* argv[])
{
    if (argc == 2)
    {
        RETURN_IF_FAILED(CoInitialize(0));

        // Get the SM, CM, and CSM
        ComPtr<ISearchManager> searchManager;
        RETURN_IF_FAILED(CoCreateInstance(CLSID_CSearchManager, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(&searchManager)));

        ComPtr<ISearchCatalogManager> catalogManager;
        RETURN_IF_FAILED(searchManager->GetCatalog(L"SystemIndex", &catalogManager));

        ComPtr<ISearchCrawlScopeManager> crawlScopeManager;
        RETURN_IF_FAILED(catalogManager->GetCrawlScopeManager(&crawlScopeManager));

        RETURN_IF_FAILED(crawlScopeManager->AddUserScopeRule(argv[1], FALSE, FALSE, FALSE));
        RETURN_IF_FAILED(crawlScopeManager->SaveAll());

        CoUninitialize();
    }

    wprintf(L"You must enter a folder to exclude!");

    return 0;
}