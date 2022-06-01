
#include "pch.h"

struct CommandLineParser : winrt::implements<CommandLineParser, ICommandLineParser>
{
    CommandLineParser(_In_reads_(numAppArgInfo) const CommandLineArgInfo* appArgInfo, uint32_t numAppArgInfo, int argc, _In_ wchar_t* argv[], _In_ PCWSTR processorName) 
        : m_appArgInfo(appArgInfo), m_numAppArgInfo(numAppArgInfo), m_numCommandArgs(argc)
    {
        // stash the command line
        for (int i = 1 /* skip the .exe */; i < argc; ++i)
        {
            m_commandArguments.push_back(std::wstring(argv[i]));
        }

        m_processorName = processorName;

        // Need at least 1 command
        if (m_commandArguments.size() == 0)
        {
            PrintHelp();
        }
    }

    ~CommandLineParser() { }

    // ICommandLineParser
    HRESULT ParseCommandLine() noexcept 
    {
        // First, validate the commands
        // We can only have a single main command, with an appropriate corresponding # of sub-commands
        bool commandFound{};
        uint32_t subCommands{};
        for (uint32_t i = 0; i < m_numAppArgInfo; ++i)
        {
            if (_wcsicmp(m_commandArguments[0].c_str(), m_appArgInfo[i].command))
            {
                commandFound = true;
                subCommands = m_appArgInfo[i].numSubArgs;
                break;
            }
        }

        if (!commandFound)
        {
            PrintHelp();
            RETURN_HR(E_INVALIDARG);
        }
        else if ((subCommands + 1) != m_commandArguments.size())
        {
            PrintHelp();
            RETURN_HR(E_INVALIDARG);
        }

        // Calls into the app specific ICommandLineProcessor to process the command line
        // If the app hasn't implemented this a linker error will be thrown for the app
        winrt::com_ptr<ICommandLineProcessor> processor;
        CreateCommandLineProcessor(processor.put());

        RETURN_IF_FAILED(processor->Process(m_commandArguments));

        return S_OK;
    }

    void PrintHelp()
    {
        wprintf(L"Help for %ws", m_processorName.c_str());

        for (uint32_t i = 0; i < m_numAppArgInfo; ++i)
        {
            wprintf(m_appArgInfo[i].helpText);
        }
    }

protected:
    CommandLineParser() { }
private:
    std::vector<std::wstring> m_commandArguments;
    uint32_t m_numCommandArgs{};
    const CommandLineArgInfo* m_appArgInfo{}; // all the sample apps will be using this format for what are supported args..we do not own this (it lives on the wmain stack)
    uint32_t m_numAppArgInfo{};
    std::wstring m_processorName;
};

void CreateCommandLineParser(_In_reads_(numArgInfo) const CommandLineArgInfo* info, uint32_t numArgInfo, int argc, _In_ wchar_t* argv[], _In_ PCWSTR processorName, _COM_Outptr_ ICommandLineParser** parser)
{
    winrt::com_ptr<ICommandLineParser> parserObj{ winrt::make<CommandLineParser>(info, numArgInfo, argc, argv, processorName).as<ICommandLineParser>() };
    parserObj.copy_to(parser);
}