/**
 * Session management
 *
 * This includes all code pertaining to
 * starting new sessions, closing them
 * and general maintenance and worker
 * functions
 */

#include "types.h"
#include<stdio.h>


#include<pthread.h>

#include <sys/mman.h>

#define _GNU_SOURCE
#include <linux/sched.h>

#include<sodium/core.h>

#include<assert.h>

/**
 * Globals
 */
Session* sessionQueue = 0;
pthread_mutex_t sessionQueueLock;

/*
 * Prototypes
 */
void initSession(Session*);
void sessionMainFunc(void*);

int32_t startListener(Session*);
int32_t startOutbound(Session*);

/**
 * Session API
 */
void lockSession(Session*);
void unlockSession(Session*);


void managementLoop();

void lockSession(Session* session)
{
    assert(session);
    pthread_mutex_lock(&session->lock);
}
void unlockSession(Session* session)
{
    assert(session);
    pthread_mutex_unlock(&session->lock);
}



/**
 * Setup all sessions
 */
void setupSessions(Session* head)
{
    /**
     * Store the head into a global variable
     * and also initialize the mutex for it
     */
    sessionQueue = head;
    pthread_mutex_init(&sessionQueueLock, 0);

    uint8_t i = 0;
    while(head)
    {
        /* Initialize the session */
        initSession(head);

        /* Move onto next session */
        head = head->next;

        i++;
    }

    //printf("Count %u\n", i);

    while(1)
    {
        //sched_yield();
        //sleep(400);
    }
}

void initSession(Session* session)
{
    assert(session);
    //printf("Initializing session '%s'...\n", session->name);

    /**
     * Flags re-used for mmap() and clone()
     */
    int flags;

    /**
     * Allocate space for the stacks of the new processes
     *
     * 0: We are not interested where in virtual address
     * space the base of the stack is
     * 1: It needs to be a multiple of page size (4096)
     * also, we really don't need too much stack space
     * at all, most of the data is in kernel queues or
     * will be on the heap. This is the absolute minimum
     * we can allocate anyways.
     * 2: The page should be readbale and writable (as
     * s stack has variables written and read from)
     * 3: MAP_ANONYMOUS, initialize to zero and this is
     * not a memeory-mapped file. We just want memory,
     * not a file read into it.
     * MAP_GROWSDOWN, whenever you touch below the extent
     * of the page it will segmentation fault but not signal,
     * rather extending and remmap (i.e. growing the stack).
     * MAP_PRIVATE, it always wants this. Not too sure why
     * as we are not mapping a file
     * 4: -1, indicating no backing fd, makes sense for MAP_ANONYMOUS
     * 5: 0, file offset N/A (TODO: check that is what it is,
     * me thinks it is)
     */
    flags = MAP_ANONYMOUS|MAP_GROWSDOWN|MAP_PRIVATE;
    void* stackBase = mmap(0, 4096, PROT_READ|PROT_WRITE, flags, -1, 0);


    /* On map success */
    if(stackBase > 0)
    {
        /* Base of page != base of stack, we are at start of page (lower)
         * and stack base would be at top of page.
         *
         * Hence apply the correct adjustment
         *
         * * TODO: Surely it should have mapped more in without this though
         * * guard hit? Looks like currrntly it is not doing so
         */
        stackBase += 4096;

        //printf("Stack for session '%s': %u\n", session->name, (uint64_t*)stackBase);



        /**
         * 1. CLONE_VM - we want to have the same virtual address space
         * but have those pages map to the same page frames
         * 2. CLONE_THREAD TODO: maybe
         */
        flags = CLONE_VM|CLONE_SIGHAND;

        /**
         * 0: the base of the stack allocated (TODO: allocate this)
         * 1: choose the base of stack for us - we don't care in
         * particular.
         * 2. flags (described above)
         */
        int sessionPID = clone(&sessionMainFunc, stackBase, flags, (void*)session);

        /* On success of thread spawn */
        if(sessionPID >= 0)
        {
            // Nothing to do here, it is now started
            // TODO: We should probably attach this PID though
            //printf("Spawned session '%s'\n", session->name);
        }
        else
        {
            //printf("Failed to spawn process for session '%s'\n", session->name);
        }
    }
    else
    {
        //printf("Could not spawn session '%s' as mmap() failed\n", session->name);
    }


}

/**
 * Main session function (seperate thread)
 *
 * FIXME: Figure out what is up with the
 * weird text printing, CLONE_FILES maybe,
 * or what?
 */
void sessionMainFunc(void* data)
{
    Session* session = (Session*)data;

    /* TODO: Cast and store data */
    printf("Hello from me %u and session '%s'\n", getpid(), session->name);


    /* TODO: Spawn two threads here and let this one just hang o provide
     * management of the sorts
     *
     * 1. Server listenr (incoming connections)
     * 2. Server outbounder (outgoing connection)
     */
    int32_t pids[2] = {0, 0}; /* TODO: I recall onstack arrays are zeroed by gcc, this is not needed */

    /* Start outbounder */
    pids[0] = startListener(session);
    pids[1] = startOutbound(session);


    if(pids[0] > 0)
    {
        //Bind
    }
    else
    {
        /* TODO: Debug print message about bind failing */
    }

    if(pids[1] > 0)
    {
        //Outbound good
    }
    else
    {
        /* TODO: Debug print message about failing to outbound */
    }

    /* Enter management loop */
    managementLoop();
}

void managementLoop()
{
    /* Management loop */
    while(1)
    {
        //sched_yield();
        //sleep(400);
    }
}


/**
 * Starts the listener for inbound connections
 * for the given Session
 */
int32_t startListener(Session* session)
{
    //
    return -1;
}

int32_t startOutbound(Session* session)
{
    //
    return -1;
}
