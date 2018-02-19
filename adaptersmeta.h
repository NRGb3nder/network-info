#pragma once

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

struct AdapterMetaInfo
{
    CHAR szDesc[512];
    CHAR szAddress[512];
};

struct AdapterMetaListNode
{
    struct AdapterMetaInfo *pInfo;
    struct AdapterMetaListNode *pNext;
};

BOOL WINAPI getAdaptersMetaInfo(struct AdapterMetaListNode *pAdapterNode);
