#include<jansson.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<stdint.h>

void test()
{
    char* configPath = "config.json";
  
    /* TODO: Remove this, this is just a test */
    int configFD = open(configPath, O_RDONLY);
  
    /* On open success */
    if(configFD >= 0)
    {
        struct stat fileInfo;
      
        stat(configPath, &fileInfo);
        uint_least8_t fileSize = fileInfo.st_size;
      
        printf("Config file size: %u\n", fileSize);
      
        u_int8_t* configBuffer = malloc(fileSize+1);
        
        /* On allocation success */
        if(configBuffer)
        {
            read(configFD, configBuffer, fileSize);
          
            /* Add null-terminator */
            *(configBuffer+fileSize+1) = 0;
          
            printf("%s", configBuffer);
        }
        
    }
    else
    {
        /* TODO: Remove this */
        printf("Opening config file failed\n");
    }
    
}
