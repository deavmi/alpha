int createTun(char* interfaceName, int iffFlags);
int destroyTun(int fd);
int tunWrite(int fd, char* data, int length);
int tunRead(int fd, char* data, int amount);
