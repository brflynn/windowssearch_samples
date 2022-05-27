
#include "pch.h"

struct CommandLineProcessor : winrt::implements<CommandLineProcessor, ICommandLineProcessor>
{
    CommandLineProcessor() { }
    ~CommandLineProcessor() { }

    // ICommandLineProcessor
    HRESULT Process(const std::vector<std::wstring>&) noexcept
    {
        return S_OK;
    }

private:

};

void CreateCommandLineProcessor(_COM_Outptr_ ICommandLineProcessor** processor)
{
    winrt::com_ptr<ICommandLineProcessor> proc{ winrt::make<CommandLineProcessor>().as<ICommandLineProcessor>() };
    proc.copy_to(processor);
}
