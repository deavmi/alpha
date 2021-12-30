#include<stdint.h>

/**
 * Session-related data type definitions
 */
struct Session
{
    //
    uint8_t* requestedInterface;
    uint8_t* privateKey;
  
    /* Next Session instance */
    struct Session* next;
};
typedef struct Session Session;
