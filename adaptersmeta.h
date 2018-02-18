#pragma once

#include <winsock2.h>
#include <iphlpapi.h>
#include <vector>
#include <string>
#pragma comment(lib, "IPHLPAPI.lib")

struct AdapterMetaInfo
{
	std::string desc;
	std::string address;
};

bool getAdaptersMetaInfo(std::vector<AdapterMetaInfo> &vMetaInfo);