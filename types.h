#include<stdint.h>
#include<pthread.h>

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
    uint8_t* publicKey;
    uint8_t* endpoint;
    uint8_t* bindAddr;
  
    /* Endpoint fd */
    int fd;
};
typedef struct Peer Peer;

/**
 * Session-related data type definitions
 */
struct Session
{
    /* Session name */
    uint8_t* name;

    /* Interface name (requested) */
    uint8_t* requestedInterface;
  
    /* Private key */
    uint8_t* privateKey;
  
    /* Listen address */
    uint8_t* listenAddr;
  
    /* Peer */
    Peer peer;
  
    /* Adapter */
    Adapter adapter;
    
    /* Hooks */
    HookSet hooks;

    /* Lock */
    pthread_mutex_t lock;

    /* Next Session instance */
    struct Session* next;
};
typedef struct Session Session;
