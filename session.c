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

#include <sys/mman.h>

#define _GNU_SOURCE
#include <linux/sched.h>


/*
 * Prototypes
 */
void initSession(Session*);
void sessionMainFunc(void*);

/**
 * Setup all sessions
 */
void setupSessions(Session* head)
{
    uint8_t i =0;
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
            //
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

    while(1)
    {
        //sched_yield();
        //sleep(400);
    }
}
