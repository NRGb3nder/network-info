#include "stdafx.h"
#include "adaptersmeta.h"

bool getAdaptersMetaInfo(std::vector<AdapterMetaInfo> &vMetaInfo)
{
	vMetaInfo.clear();

	IP_ADAPTER_INFO aAdapterInfo[32];
	PIP_ADAPTER_INFO pAdapterInfo = aAdapterInfo;
	DWORD dwBufLen = sizeof(aAdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(pAdapterInfo, &dwBufLen);

	if (dwStatus != ERROR_SUCCESS) {
		return false;
	}

	char szBuffer[512];
	while (pAdapterInfo) {
		if (pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET) {
			AdapterMetaInfo adapter;

			sprintf_s(szBuffer, sizeof(szBuffer), "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
				pAdapterInfo->Address[0],
				pAdapterInfo->Address[1],
				pAdapterInfo->Address[2],
				pAdapterInfo->Address[3],
				pAdapterInfo->Address[4],
				pAdapterInfo->Address[5]
			);
			adapter.address = szBuffer;

			sprintf_s(szBuffer, sizeof(szBuffer), "%s", pAdapterInfo->Description);
			adapter.desc = szBuffer;

			vMetaInfo.push_back(adapter);
		}
		pAdapterInfo = pAdapterInfo->Next;
	}

	return true;
}