#include <windows.h>
#include <winnetwk.h>
#pragma comment(lib, "mpr.lib")

BOOL WINAPI enumerateResources(LPNETRESOURCE lpnr, FILE *errlog = NULL, DWORD dwNesting = 0);