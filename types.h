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
