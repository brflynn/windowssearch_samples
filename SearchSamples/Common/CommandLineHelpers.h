#pragma once

#include <cstdint>

struct __declspec(uuid("09ae410c-49e9-48cc-9eb7-4a3cf6b3b9ac")) ICommandLineProcessor : IInspectable
{
    virtual HRESULT Process(const std::vector<std::wstring>& commandLine) noexcept = 0;
};

struct __declspec(uuid("296f3610-03d6-430a-90d9-8137a926790d")) ICommandLineParser : IInspectable
{
    virtual HRESULT ParseCommandLine() noexcept = 0;
};

struct CommandLineArgInfo
{
    const wchar_t* command; // The actual command
    uint32_t numSubArgs; // The total number of sub-arguments expected after the command
    const wchar_t* helpText; // The help text associated with the command
};

void CreateCommandLineParser(_In_reads_(numArgInfo) const CommandLineArgInfo* info, uint32_t numArgInfo, int argc, _In_ wchar_t* argv[], _In_ PCWSTR processorName, _COM_Outptr_ ICommandLineParser** parser);
void CreateCommandLineProcessor(_COM_Outptr_ ICommandLineProcessor** processor); // Just define the method, the applications will need to implement it if they choose to use a processor (will get a linker error)
