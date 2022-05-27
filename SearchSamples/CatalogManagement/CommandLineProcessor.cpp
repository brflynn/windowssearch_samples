#include "pch.h"

struct CommandLineProcessor : winrt::implements<CommandLineProcessor, ICommandLineProcessor>
{
    CommandLineProcessor()
    {
        // cache what we will need later...
         // Get the SM, CM, and CSM
        winrt::com_ptr<ISearchManager> searchManager;
        THROW_IF_FAILED(CoCreateInstance(CLSID_CSearchManager, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(m_searchManager.put())));
    }
    ~CommandLineProcessor() { }

    // ICommandLineProcessor
    HRESULT Process(const std::vector<std::wstring>& commandLine) noexcept
    {
        // The command line was already validated if we get here...
        if (_wcsicmp(commandLine[0].c_str(), L"/r") == 0)
        {
            // Catalog reset, which catalog?
            winrt::com_ptr<ISearchCatalogManager> catalogManager;
            RETURN_IF_FAILED(m_searchManager->GetCatalog(commandLine[2].c_str(), catalogManager.put()));

            wprintf(L"Resetting the %s catalog.", commandLine[2].c_str());
            RETURN_IF_FAILED(catalogManager->Reset());
        }
        return S_OK;
    }

private:
    winrt::com_ptr<ISearchManager> m_searchManager;

};

void CreateCommandLineProcessor(_COM_Outptr_ ICommandLineProcessor** processor)
{
    winrt::com_ptr<ICommandLineProcessor> proc{ winrt::make<CommandLineProcessor>().as<ICommandLineProcessor>() };
    proc.copy_to(processor);
}