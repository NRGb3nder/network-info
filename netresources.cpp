#include "stdafx.h"
#include "netresources.h"

void logerror(FILE *errlog, CONST CHAR *szMsg);
void logerrorcode(FILE *errlog, CONST CHAR *szMsg, DWORD dwErrcode);
void showResource(DWORD dwNesting, LPNETRESOURCE lpnrLocal);
BOOL WINAPI _enumerateResources(LPNETRESOURCE lpnr, FILE *errlog, DWORD dwNesting);

BOOL WINAPI enumerateResources()
{
    LPNETRESOURCE lpnr = NULL;

    return _enumerateResources(lpnr, NULL, 0);
}

BOOL WINAPI _enumerateResources(LPNETRESOURCE lpnr, FILE *errlog, DWORD dwNesting)
{
    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;
    DWORD cEntries = -1;
    LPNETRESOURCE lpnrLocal;
    DWORD i;

    dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, lpnr, &hEnum);
    if (dwResult != NO_ERROR) {
        logerrorcode(errlog, "WNetOpenEnum failed with error %d\n", dwResult);
        return FALSE;
    }

    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
    if (lpnrLocal == NULL) {
        logerrorcode(errlog, "WNetOpenEnum failed with error %d\n", dwResult);
        return FALSE;
    }

    do {
        ZeroMemory(lpnrLocal, cbBuffer);
        dwResultEnum = WNetEnumResource(hEnum, &cEntries, lpnrLocal, &cbBuffer);
        if (dwResultEnum == NO_ERROR) {
            for (i = 0; i < cEntries; i++) {
                showResource(dwNesting, &lpnrLocal[i]);
                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER)) {
                    if (!_enumerateResources(&lpnrLocal[i], errlog, dwNesting + 1)) {
                        logerror(errlog, "enumerateResources function returned FALSE\n");
                    }
                }
            }
        }
        else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
            logerrorcode(errlog, "WNetEnumResource failed with error %d\n", dwResultEnum);
            break;
        }
    } while (dwResultEnum != ERROR_NO_MORE_ITEMS);

    GlobalFree((HGLOBAL)lpnrLocal);
    dwResult = WNetCloseEnum(hEnum);

    if (dwResult != NO_ERROR) {
        logerrorcode(errlog, "WNetCloseEnum failed with error %d\n", dwResult);
        return FALSE;
    }

    return TRUE;
}

void logerror(FILE *errlog, CONST CHAR *szMsg)
{
    if (errlog) {
        fprintf(errlog, szMsg);
    }
}

void logerrorcode(FILE *errlog, CONST CHAR *szMsg, DWORD dwErrcode)
{
    if (errlog) {
        fprintf(errlog, szMsg, dwErrcode);
    }
}

void showResource(DWORD dwNesting, LPNETRESOURCE lpnrLocal)
{
    DWORD i;

    printf("-");
    for (i = 0; i < dwNesting; i++) {
        printf("--");
    }
    printf("> ");

    printf("%S ", lpnrLocal->lpRemoteName);
    if (lpnrLocal->lpComment) {
        printf("(%S) ", lpnrLocal->lpComment);
    }

    printf("{type: ");
    switch (lpnrLocal->dwType) {
    case (RESOURCETYPE_ANY):
        printf("any");
        break;
    case (RESOURCETYPE_DISK):
        printf("disk");
        break;
    case (RESOURCETYPE_PRINT):
        printf("print");
        break;
    default:
        printf("unknown %X", lpnrLocal->dwDisplayType);
        break;
    }
    printf(", display type: ");
    switch (lpnrLocal->dwDisplayType) {
    case (RESOURCEDISPLAYTYPE_GENERIC):
        printf("generic");
        break;
    case (RESOURCEDISPLAYTYPE_DOMAIN):
        printf("domain");
        break;
    case (RESOURCEDISPLAYTYPE_SERVER):
        printf("server");
        break;
    case (RESOURCEDISPLAYTYPE_SHARE):
        printf("share");
        break;
    case (RESOURCEDISPLAYTYPE_FILE):
        printf("file");
        break;
    case (RESOURCEDISPLAYTYPE_GROUP):
        printf("group");
        break;
    case (RESOURCEDISPLAYTYPE_NETWORK):
        printf("network");
        break;
    default:
        printf("unknown %X", lpnrLocal->dwDisplayType);
        break;
    }
    printf("}\n");
}
