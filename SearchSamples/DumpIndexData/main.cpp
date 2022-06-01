// DumpIndexData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using namespace winrt::Windows::Foundation;


const CommandLineArgInfo c_commandInfo[] =
{
    { L"\n/items", 0, L"/items\t\t- Dumps all items in the index" },
    { L"\n/items", 1, L"/items\t\"protocol\"\t- Dumps all excluded scope URLs" },
    { L"\n/props", 0, L"/props\t\t- Dumps all properties to a props.txt file"},
};



int wmain(int argc, wchar_t* argv[])
{
    winrt::init_apartment();

    // Just need to create the command line parser, it will try to create the processor that we must implement
    try
    {
        winrt::com_ptr<ICommandLineParser> parser;
        CreateCommandLineParser(c_commandInfo, ARRAYSIZE(c_commandInfo), argc, argv, L"DumpIndexDataSample", parser.put());

        THROW_IF_FAILED(parser->ParseCommandLine());
    }
    CATCH_LOG();
}