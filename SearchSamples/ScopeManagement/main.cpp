#include "pch.h"

using namespace winrt::Windows::Foundation;


const CommandLineArgInfo c_commandInfo[] =
{
    { L"/dinc", 0, L"/dinc      - Dumps all included scope URLs" },
    { L"/dex",  0, L"/dex       - Dumps all excluded scope URLs" },
    { L"/c",    1, L"/c \"url\" - Checks to see if a url is included. URLs are accepted in folder path form. E.g. \"C:\\users\\foo\\bar\""},
};



int wmain(int argc, wchar_t* argv[])
{
    winrt::init_apartment();

    // Just need to create the command line parser, it will try to create the processor that we must implement
    try
    {
        winrt::com_ptr<ICommandLineParser> parser;
        CreateCommandLineParser(c_commandInfo, ARRAYSIZE(c_commandInfo), argc, argv, parser.put());

        THROW_IF_FAILED(parser->ParseCommandLine());
    }
    CATCH_LOG();

}
