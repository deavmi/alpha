#include<stdint.h>

struct Adapter
{
    /* Final adapter name */
    char* interfaceName;
  
    /* fd to tun/tap devie */
    int fd;
  
    /* Next Adapter */
    struct Adapter* next;
};
typedef struct Adapter Adapter;


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
    
    
    /* Hooks */
    HookSet hooks;

    /* Next Session instance */
    struct Session* next;
};
typedef struct Session Session;
