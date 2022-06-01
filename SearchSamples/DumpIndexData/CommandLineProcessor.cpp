#include "pch.h"

struct CommandLineProcessor : winrt::implements<CommandLineProcessor, ICommandLineProcessor>
{
    CommandLineProcessor() { }
    ~CommandLineProcessor() { }

    // ICommandLineProcessor
    HRESULT Process(const std::vector<std::wstring>& commandLine) noexcept
    {
        // The command line was already validated if we get here...
        if (_wcsicmp(commandLine[0].c_str(), L"/items") == 0)
        {
            // Dump items...is there a protocol?

        }
        else if (_wcsicmp(commandLine[0].c_str(), L"/props") == 0)
        {
            wprintf(L"\nEnumerating all plain text properties in the index.");
            std::vector<std::wstring> propList;
            RETURN_IF_FAILED(GetIndexedColumnProperties(propList));

            for (DWORD i = 0; i < propList.size(); ++i)
            {
                std::wstring queryStr{ L"SELECT " };
                wprintf(L"\n%ws metadata\n", propList[i].c_str());
                wprintf(L"-----------------------------------------------------------");
                queryStr += L'"';
                queryStr += propList[i].c_str();
                queryStr += L'"';

                queryStr += L" FROM SystemIndex";
                CIndexerQueryHelper queryHelper;
                queryHelper.RunSql(queryStr.c_str());

                std::vector<PropertyValueInfo> propInfo;
                queryHelper.FetchRows(propInfo);

                // We should have all the property values now, go ahead and print them
                for (const auto it : propInfo)
                {
                    wprintf(L"\n%ws: %ws", it.propertyName.c_str(), it.propertyValue.c_str());
                }
            }
        }
        return S_OK;
    }

private:

};

void CreateCommandLineProcessor(_COM_Outptr_ ICommandLineProcessor** processor)
{
    winrt::com_ptr<ICommandLineProcessor> proc{ winrt::make<CommandLineProcessor>().as<ICommandLineProcessor>() };
    proc.copy_to(processor);
}