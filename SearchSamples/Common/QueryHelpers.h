#pragma once


// Helper class to execute queries that encapsulates most of the ICommand overhead
// This class throws

struct PropertyValueInfo
{
    std::wstring propertyName;
    std::wstring propertyValue;
};

__declspec(selectany) CLSID CLSID_CollatorDataSource = { 0x9E175B8B, 0xF52A, 0x11D8, 0xB9, 0xA5, 0x50, 0x50, 0x54, 0x50, 0x30, 0x30 };

class CIndexerQueryHelper
{
public:
    CIndexerQueryHelper() {}

    CIndexerQueryHelper(_In_ PCWSTR pSql) { RunSql(pSql); }

    void RunSql(_In_ PCWSTR pSql)
    {
        winrt::com_ptr<IDBCreateCommand> createCommand;
        _GetCommandCreate(createCommand.put());

        winrt::com_ptr<ICommandText> command;
        winrt::com_ptr<IUnknown> unknown;

        // Some interface we are using here require IUnknown ** with different IID, and that is why weirdness like below
        // is adopted
        THROW_IF_FAILED(createCommand->CreateCommand(0, __uuidof(*(command.get())), unknown.put()));
        unknown.as(command);

        THROW_IF_FAILED(command->SetCommandText(DBGUID_DEFAULT, pSql));

        DBROWCOUNT rowCount = 0;
        if (FAILED(command->Execute(nullptr, __uuidof(*(m_rowset.get())), nullptr, &rowCount, unknown.put())))
        {
            wprintf(L"%ws query failed to execute. Bad column.", pSql);
            return;
        }
        else
        {
            unknown.as(m_rowset);
        }
    }

    void FetchRows(_Inout_ std::vector<PropertyValueInfo>& props)
    {
        if (!m_rowset)
        {
            return;
        }

        winrt::com_ptr<IGetRow> getRow;
        m_rowset.as(getRow);

        bool continueFetch = true;
        DBCOUNTITEM rowCountReturned;

        do
        {
            HROW rowBuffer[100]; // Request enough large batch to increase efficiency
            HROW* rowReturned = rowBuffer;

            THROW_IF_FAILED(
                m_rowset->GetNextRows(DB_NULL_HCHAPTER, 0, ARRAYSIZE(rowBuffer), &rowCountReturned, &rowReturned));

            for (unsigned int i = 0; continueFetch && (i < rowCountReturned); i++)
            {
                winrt::com_ptr<IPropertyStore> propStore;
                winrt::com_ptr<IUnknown> unknown;

                THROW_IF_FAILED(getRow->GetRowFromHROW(nullptr, rowBuffer[i], __uuidof(*(propStore.get())), unknown.put()));
                unknown.as(propStore);

                DWORD propCount{};
                THROW_IF_FAILED(propStore->GetCount(&propCount));

                for (DWORD propIndex = 0; propIndex < propCount; ++propIndex)
                {
                    PROPERTYKEY key;
                    THROW_IF_FAILED(propStore->GetAt(0, &key));

                    wil::unique_prop_variant propVar;
                    THROW_IF_FAILED(propStore->GetValue(key, propVar.reset_and_addressof()));

                    if ((propVar.vt == VT_LPWSTR)
                        && (propVar.pwszVal != nullptr) && (propVar.pwszVal[0] != L'\0')
                        && (propVar.pwszVal != L""))
                    {
                        std::wstring propName{};
                        THROW_IF_FAILED(GetPropertyNameFromKey(key, propName));

                        PropertyValueInfo info{};
                        info.propertyName = std::move(propName);
                        info.propertyValue = propVar.pwszVal;
                        props.push_back(std::move(info));
                    }
                }
            }

            THROW_IF_FAILED(m_rowset->ReleaseRows(rowCountReturned, rowReturned, nullptr, nullptr, nullptr));
        } while (continueFetch && (rowCountReturned > 0));
    }

protected:
    winrt::com_ptr<IRowset> m_rowset;

    virtual void HandleCommandProperties(_In_ ICommandText*) { }

private:
    static void _GetCommandCreate(_Outptr_ IDBCreateCommand** commandOut)
    {
        winrt::com_ptr<IDBInitialize> dataSource;
        THROW_IF_FAILED(
            CoCreateInstance(CLSID_CollatorDataSource, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dataSource.put())));
        THROW_IF_FAILED(dataSource->Initialize());

        winrt::com_ptr<IDBCreateSession> session;
        dataSource.as(session);

        winrt::com_ptr<IDBCreateCommand> command;
        winrt::com_ptr<IUnknown> unknown;
        THROW_IF_FAILED(session->CreateSession(0, __uuidof(*(command.get())), unknown.put()));
        THROW_IF_FAILED(unknown->QueryInterface(IID_PPV_ARGS(commandOut))); // copy the command out
    }
};
