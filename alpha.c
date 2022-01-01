#include<stdio.h>
#include<stdlib.h>
#include "libtun.h"
#include<stdint.h>
#include "config.h"

/**
 * Prototyopes
 */
Adapter* createDevice(char*);
void setupAdapters(Session*);

int main()
{
    printf("Starting alphavpn...\n");


    Session* sessionHead = getSessionConfig();
    if(sessionHead)
    {
        setupAdapters(sessionHead);
    }
    else
    {
        printf("Error when reading configuration file\n");
    }

}

/**
 * Setup the adapters for each session
 */
void setupAdapters(Session* session)
{
    while(session)
    {
        // TODO: Implement me
        Adapter* newAdapter = createDevice(session->requestedInterface);

        /**
         * If the creation of a new Adapter succeeds,
         * i.e. the allocation of a new TUN or TAP adapter
         * if successful, then attach this adapter to
         * its related Session
         */
        if(newAdapter)
        {
            session->adapter = *newAdapter;
        }

        /* Move onto next Session */
        session = session->next;
    }
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
        adapter->interfaceName = name;

        /* NULL out `next` ptr */
        adapter->next = NULL;

        return adapter;
    }
    else
    {
        /* TODO: Remove this later */
        printf("Allocation failure for device name '%s'!", name);
        return 0;
    }
}
