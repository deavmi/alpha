#include<stdio.h>
#include<stdlib.h>
#include "types.h"
#include "libtun.h"

/**
 * *Prototyopes
 */
Adapter* createDevice(char*);


/**
 * Global session state management
 */
Adapter* adapterQueue = NULL;

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
 *
 * TODO: Add ifType argument,
 * `TAP` or `TUN`?
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

        /* NULL out `next` ptr */
        adapter->next = NULL;
    }
    else
    {
        /* TODO: Remove this later */
        printf("Allocation failure for device name '%s'!", name);
    }

    return adapter;
}

/**
 * Add the given Adapter to the list of
 * adapters
 */
void addAdapter(Adapter* adapter)
{
    /* Only add the adapter if it isn't NULL */
    if(adapter)
    {
        Adapter* current = adapterQueue;

        /**
         * If the wueue is empty then we
         * make the head of the queue the
         * adapter to be added
         */
        if(adapterQueue == NULL)
        {
            adapterQueue = adapter;
        }
        /**
         * Else, we iterate till the tail
         * of the adapter queue and add it
         * to the tail
         */
        else
        {
            while(current->next)
            {
                current = current->next;
            }

            current->next = adapter;
        }

    }
}
