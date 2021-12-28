#include<stdio.h>
#include<stdlib.h>
#include "types.h"

/**
 * *Prototyopes
 */
Adapter* createDevice(char*);


/**
 * Global session state management
 */
Adapter* adapterQueue;

int main()
{
    printf("Starting alphavpn...\n");

    /**
     * TODO: Testing code
     * 1. Create a device named `alpha0`
     */
    createDevice("alpha0");

}


/**
 * Spawns a new TUN device wrapped
 * in an Adapter struct, on failure
 * NULL is returned
 */
Adapter* createDevice(char* name)
{
    Adapter* adapter = malloc(sizeof(adapter));

    /* On allocation success */
    if(adapter)
    {
        /* TODO: Fetch tun fd */
        adapter->fd = 0;

        /* TODO: Fetch final name */
        adapter->interfaceName = "poes";
    }
    else
    {
        /* TODO: Remove this later */
        printf("Allocation failure for device name '%s'!", name);
    }

    return adapter;
}
