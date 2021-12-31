#include<stdint.h>

/**
 * Hooks
 */
struct HookSet
{
    char** values;
    char** keys;
};

typedef struct HookSet HookSet;

/**
 * Peer details
 */
struct Peer
{
    //
};
typedef struct Peer Peer;

/**
 * Session-related data type definitions
 */
struct Session
{
    /* Interface name (requested) */
    uint8_t* requestedInterface;
  
    /* Private key */
    uint8_t* privateKey;
  
    /* Listen address */
    uint8_t* listenAddr;
  
    /* Peer */
    Peer peer;
  
    /* TODO: Add Adapter here */
    //
    
    /* Hooks */
    HookSet hooks;

    /* Next Session instance */
    struct Session* next;
};
typedef struct Session Session;
