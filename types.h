struct Adapter
{
    /* Final adapter name */
    char* interfaceName;
  
    /* fd to tun/tap devie */
    int fd;
};
typedef struct Adapter Adapter;
