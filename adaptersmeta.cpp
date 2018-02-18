#include "stdafx.h"
#include "adaptersmeta.h"

BOOL getAdaptersMetaInfo(struct AdapterMetaListNode *pAdapterNode)
{
	IP_ADAPTER_INFO aAdapterInfo[32];
	PIP_ADAPTER_INFO pAdapterInfo = aAdapterInfo;
	DWORD dwBufLen = sizeof(aAdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(pAdapterInfo, &dwBufLen);

	if (dwStatus != ERROR_SUCCESS) {
		return FALSE;
	}

	while (pAdapterInfo) {
		if (pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET) {
			pAdapterNode->pNext = (struct AdapterMetaListNode *) malloc(sizeof(struct AdapterMetaListNode));
			pAdapterNode = pAdapterNode->pNext;
			pAdapterNode->pInfo = (struct AdapterMetaInfo *) malloc(sizeof(struct AdapterMetaInfo));
			pAdapterNode->pNext = NULL;

			sprintf_s(pAdapterNode->pInfo->szAddress, sizeof(pAdapterNode->pInfo->szAddress), 
				"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
				pAdapterInfo->Address[0],
				pAdapterInfo->Address[1],
				pAdapterInfo->Address[2],
				pAdapterInfo->Address[3],
				pAdapterInfo->Address[4],
				pAdapterInfo->Address[5]
			);

			sprintf_s(pAdapterNode->pInfo->szDesc, sizeof(pAdapterNode->pInfo->szDesc),
				"%s", pAdapterInfo->Description);
		}
		pAdapterInfo = pAdapterInfo->Next;
	}

	return TRUE;
}