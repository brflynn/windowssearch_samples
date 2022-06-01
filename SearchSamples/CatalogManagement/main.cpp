#include "pch.h"

using namespace winrt::Windows::Foundation;

#pragma comment(lib, "SearchSDK.lib")

const CommandLineArgInfo c_commandInfo[] =
{
    { L"/r", 1, L"/r  \"SystemIndex\"  - Resets the catalog with the given name. This deletes all the data and forces a rebuild of the catalog." },
};

int wmain(int argc, wchar_t* argv[])
{
    winrt::init_apartment();

    // Just need to create the command line parser, it will try to create the processor that we must implement
    try
    {
        winrt::com_ptr<ICommandLineParser> parser;
        CreateCommandLineParser(c_commandInfo, ARRAYSIZE(c_commandInfo), argc, argv, L"CatalogManagementSample", parser.put());

        THROW_IF_FAILED(parser->ParseCommandLine());
    }
    CATCH_LOG();

}