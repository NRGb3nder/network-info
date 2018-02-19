#include "stdafx.h"
#include "netresources.h"

#define NO_ERR_CODE 0xFFFF

void logerror(FILE *errlog, CONST CHAR *szMsg, DWORD dwErrcode = 0xFFFF);
void showResource(DWORD dwNesting, LPNETRESOURCE lpnrLocal);

BOOL WINAPI enumerateResources(LPNETRESOURCE lpnr, FILE *errlog, DWORD dwNesting)
{
    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;
    DWORD cEntries = -1;
    LPNETRESOURCE lpnrLocal;
    DWORD i;

    dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, lpnr, &hEnum);
    if (dwResult != NO_ERROR) {
        logerror(errlog, "WNetOpenEnum failed with error %d\n", dwResult);
        return FALSE;
    }

    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
    if (lpnrLocal == NULL) {
        logerror(errlog, "WNetOpenEnum failed with error %d\n", dwResult);
        return FALSE;
    }

    do {
        ZeroMemory(lpnrLocal, cbBuffer);
        dwResultEnum = WNetEnumResource(hEnum, &cEntries, lpnrLocal, &cbBuffer);
        if (dwResultEnum == NO_ERROR) {
            for (i = 0; i < cEntries; i++) {
                showResource(dwNesting, &lpnrLocal[i]);
                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER)) {
                    if (!enumerateResources(&lpnrLocal[i], errlog, dwNesting + 1)) {
                        logerror(errlog, "enumerateResources function returned FALSE\n");
                    }
                }
            }
        }
        else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
            logerror(errlog, "WNetEnumResource failed with error %d\n", dwResultEnum);
            break;
        }
    } while (dwResultEnum != ERROR_NO_MORE_ITEMS);

    GlobalFree((HGLOBAL)lpnrLocal);
    dwResult = WNetCloseEnum(hEnum);

    if (dwResult != NO_ERROR) {
        logerror(errlog, "WNetCloseEnum failed with error %d\n", dwResult);
        return FALSE;
    }

    return TRUE;
}

void logerror(FILE *errlog, CONST CHAR *szMsg, DWORD dwErrcode)
{
    if (errlog) {
        if (dwErrcode == NO_ERR_CODE) {
            fprintf(errlog, szMsg, dwErrcode);
        } else {
            fprintf(errlog, szMsg);
        }
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
