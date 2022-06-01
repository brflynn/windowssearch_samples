#pragma once




inline HRESULT GetPropertyStoreEnumeration(_Outptr_ IPropertyDescriptionList** propList)
{
    winrt::com_ptr<IPropertySystem> propsys;

    RETURN_IF_FAILED(CoCreateInstance(CLSID_PropertySystem, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&propsys)));
    RETURN_IF_FAILED(propsys->EnumeratePropertyDescriptions(PDEF_ALL, IID_PPV_ARGS(propList)));

    return S_OK;
}

inline HRESULT GetPropertyNameFromKey(const PROPERTYKEY& key, _Inout_ std::wstring propName)
{
    winrt::com_ptr<IPropertySystem> propsys;

    RETURN_IF_FAILED(CoCreateInstance(CLSID_PropertySystem, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&propsys)));

    winrt::com_ptr<IPropertyDescription> propDesc;
    RETURN_IF_FAILED(propsys->GetPropertyDescription(key, IID_PPV_ARGS(propDesc.put())));

    PWSTR canonicalName;
    RETURN_IF_FAILED(propDesc->GetCanonicalName(&canonicalName));

    propName = canonicalName;

    return S_OK;
}


inline HRESULT GetIndexedColumnProperties(_Inout_ std::vector<std::wstring>& propListOut)
{
    winrt::com_ptr<IPropertyDescriptionList> propList;
    RETURN_IF_FAILED(GetPropertyStoreEnumeration(propList.put()));

    UINT propCount = 0;
    RETURN_IF_FAILED(propList->GetCount(&propCount));

    // Loop through all our properties as long as we do not need to do a full rebuild
    for (UINT i = 0; i < propCount; ++i)
    {
        winrt::com_ptr<IPropertyDescription> propDescription;
        RETURN_IF_FAILED(propList->GetAt(i, IID_PPV_ARGS(propDescription.put())));

        // First it needs to be a property we care about
        winrt::com_ptr<IPropertyDescriptionSearchInfo> propDescriptionSearchInfo;
        propDescription.as(propDescriptionSearchInfo);

        PROPDESC_SEARCHINFO_FLAGS searchInfoFlags = PDSIF_DEFAULT;
        RETURN_IF_FAILED(propDescriptionSearchInfo->GetSearchInfoFlags(&searchInfoFlags));

        if (WI_IsAnyFlagSet(
            searchInfoFlags, PDSIF_ISCOLUMN)) // need to be a property we care about
        {
            PROPERTYKEY pkey;
            RETURN_IF_FAILED(propDescription->GetPropertyKey(&pkey));

            PWSTR canonicalName{};
            RETURN_IF_FAILED(propDescription->GetCanonicalName(&canonicalName));

            propListOut.push_back(std::move(std::wstring(canonicalName)));
        }
    }

    return S_OK;
}