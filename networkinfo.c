#include "stdafx.h"
#include "adaptersmeta.h"
#include "netresources.h"

int main()
{
    struct AdapterMetaListNode *pAdapterList;
    struct AdapterMetaListNode *pCurrentNode;

    pAdapterList = (struct AdapterMetaListNode *) malloc(sizeof(struct AdapterMetaListNode));
    pAdapterList->pInfo = NULL;
    pAdapterList->pNext = NULL;

    if (!getAdaptersMetaInfo(pAdapterList)) {
        printf("Can not get adapters info\n");
    } else {
        printf("MAC ADDRESSES:\n");
        pCurrentNode = pAdapterList;
        while (pCurrentNode->pNext) {
            pCurrentNode = pCurrentNode->pNext;
            printf("\n%s\n%s\n", pCurrentNode->pInfo->szDesc,
                pCurrentNode->pInfo->szAddress);
        }
        printf("\n");
    }

    printf("NETWORK RESOURCES:\n\n");
    if (!enumerateResources()) {
        printf("Call to enumerateResources function failed");
    }

    while (pCurrentNode = pAdapterList) {
        pAdapterList = pAdapterList->pNext;
        if (pCurrentNode->pInfo) {
            free(pCurrentNode->pInfo);
        }
        free(pCurrentNode);
    }

    printf("\n");
    system("pause");
    return 0;
}
