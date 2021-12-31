#include<stdint.h>

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
  
    /* Next Session instance */
    struct Session* next;
};
typedef struct Session Session;
