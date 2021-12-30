#include<jansson.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<stdint.h>
#include "session.h"

/**
 * Prototypes
 */
void parseConfig(json_t*);

void test()
{
    u_int8_t* configPath = "config.json";
  
    /* TODO: Remove this, this is just a test */
    int configFD = open(configPath, O_RDONLY);
  
    /* On open success */
    if(configFD >= 0)
    {
        struct stat fileInfo;
      
        /* On stat() success */
        if(stat(configPath, &fileInfo) == 0)
        {
            /* FIXME: `uint_least8_t` didn't work, it stayed small */
            uint64_t fileSize = fileInfo.st_size;
      
            printf("Config file size: %u\n", fileSize);
          
            uint8_t* configBuffer = malloc(fileSize+1);
            
            /* On allocation success */
            if(configBuffer)
            {
                /* Read in configuration */
                read(configFD, configBuffer, fileSize);
              
                /* Add null-terminator */
                *(configBuffer+fileSize+1) = 0;
              
                /* Load the configuration */
                json_error_t jsonError;
                json_t* rootJSON = json_loads(configBuffer, 0, &jsonError);
              
                /* On JSON parse success */
                if(rootJSON)
                {
                    /* Parse the configuration */
                    parseConfig(rootJSON);
                }
                /* On JSON parse failure */
                else
                {
                    printf("Error parsing configuration file: %s\n", jsonError.text);
                }   
            }
            else
            {
                printf("Error allocating config file memory buffer\n");
            }
        }
        /* On stat() failure */
        else
        {
            /* TODO: Handle this */
            printf("Error stat()'ing file\n");
        }
        
        
        
    }
    else
    {
        /* TODO: Remove this */
        printf("Opening config file failed\n");
    }
    
}



/**
 * Parses the JSON configuration given the
 * root of the JSON structure
 */
void parseConfig(json_t* configJSON)
{  
    uint8_t* key; /* TODO: Figure out const meaning */
    json_t* value;
    json_object_foreach (configJSON, key, value)
    {
        // Iterate here
        printf("Key %s\n", key);
      
        Session* newSession = malloc(sizeof(Session));
      
        /* TODO: Check for errors here (throughout the whole thing) */
        if(newSession)
        {
            /* Fetch the session object */
            json_t* sessionObject = json_object_get(configJSON, key);
          
            /* Fetch the interface name */
            json_t* interfaceJSON = json_object_get(sessionObject, "interface");
            if(interfaceJSON)
            {
                /* TODO: Handle type error */
                uint8_t* interfaceNameRequested = json_string_value(interfaceJSON);
                printf("Interface name: %s\n", interfaceNameRequested);
            }
            else
            {
                /* TODO: Handle error here, json error? */
                printf("Couldn't find `interfaceName` declaration for session %s\n", key);
            }
            
            //newSession->requestedInterface = 
        }
        else
        {
            //TODO: Error allocating
        }
    }
}
