#include<jansson.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<stdint.h>
#include "types.h"

/**
 * Prototypes
 */
Session* parseConfig(json_t*);

Session* getSessionConfig()
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
                    return parseConfig(rootJSON);
                }
                /* On JSON parse failure */
                else
                {
                    printf("Error parsing configuration file: %s\n", jsonError.text);
                    return 0;
                }   
            }
            else
            {
                printf("Error allocating config file memory buffer\n");
                return 0;
            }
        }
        /* On stat() failure */
        else
        {
            /* TODO: Handle this */
            printf("Error stat()'ing file\n");
            return 0;
        }
    }
    else
    {
        /* TODO: Remove this */
        printf("Opening config file failed\n");
        return 0;
    }
}



/**
 * Parses the JSON configuration given the
 * root of the JSON structure
 */
Session* parseConfig(json_t* configJSON)
{
    Session* head = 0;
    Session* last = 0;
  

    uint8_t* key; /* TODO: Figure out const meaning */
    json_t* value;
    json_object_foreach (configJSON, key, value)
    {
        // Iterate here
        printf("Key %s\n", key);
      
        Session* newSession = malloc(sizeof(Session));
        
        
        
        /* Set the head */
        if(head == 0)
        {
            head = newSession;
            last = head;
        }
        /* Set the current Session as the last Sessoin's sucessor */
        else
        {
            last->next = newSession;
        }
      
        /* TODO: Check for errors here (throughout the whole thing) */
        if(newSession)
        {
          
            /* Set the Session's name */
            newSession->name = key;
          
            /**
             * The `listenAddr` field may be empty
             * and so may the `peer.endpoint` and
             * `peer.bind` fields. So zero these out.
             * 
             * The hooks may also be zeroed (optional)
             * therefore `hooks.values` and `hooks.keys`
             */
            newSession->listenAddr = 0;
            newSession->peer.endpoint = 0;
            newSession->peer.bindAddr = 0;
            newSession->hooks.values = 0;
            newSession->hooks.keys = 0;
          

          
            /* Fetch the session object */
            json_t* sessionObject = json_object_get(configJSON, key);
          
            /* Fetch the interface name */
            json_t* interfaceJSON = json_object_get(sessionObject, "interface");
            if(interfaceJSON)
            {
                /* Fetch interface name */
                uint8_t* interfaceNameRequested = json_string_value(interfaceJSON);
                if(interfaceNameRequested)
                {
                    printf("Interface name: %s\n", interfaceNameRequested);
                    newSession->requestedInterface = interfaceNameRequested;
                }
                else
                {
                    printf("`interface` must be a string\n");
                    return 0;
                }
            }
            else
            {
                printf("Couldn't find `interfaceName` declaration for session %s\n", key);
                return 0;
            }
          
            /* Fetch the private key */
            json_t* privateKeyJSON = json_object_get(sessionObject, "privateKey");
            if(privateKeyJSON)
            {
                /* Fetch private key */
                uint8_t* privateKey = json_string_value(privateKeyJSON);
                if(privateKey)
                {
                    printf("Private key: %s\n", privateKey);
                    newSession->privateKey = privateKey;
                }
                else
                {
                    printf("`privateKey` must be a string\n");
                    return 0;
                }
            }
            else
            {
                printf("Couldn't find `privateKey` declaration for session %s\n", key);
                return 0;
            }
          
            /* Fetch the listen address */
            json_t* listenAddrJSON = json_object_get(sessionObject, "listen");
            if(listenAddrJSON)
            {
                /* Fetch listen address */
                uint8_t* listenAddress = json_string_value(listenAddrJSON);
                if(listenAddress)
                {
                    printf("Listen addressL %s\n", listenAddress);
                    newSession->listenAddr = listenAddress;
                }
                else
                {
                    printf("`listenAddress` must be a string\n");
                    return 0;
                }
            }
            else
            {
                printf("Couldn't find `listen` declaration for session %s\n", key);
                return 0;
            }
          
            /* Fetch the peer block */
            json_t* peerBlock = json_object_get(sessionObject, "peer");
            if(peerBlock)
            {
                /* Fetch the publicKey */
                json_t* publicKeyJSON = json_object_get(peerBlock, "publicKey");
                if(publicKeyJSON)
                {
                    /* Fetch the public key */
                    uint8_t* publicKey = json_string_value(publicKeyJSON);
                    if(publicKey)
                    {
                        printf("Public key: %s\n", publicKey);
                        newSession->peer.publicKey = publicKey;
                    }
                    else
                    {
                        printf("`publicKey` must be a string\n");
                        return 0;
                    }
                }
                else
                {
                    printf("Missing public key for session '%s'\n", key);
                    return 0;
                }
                
                /* Fetch peer.endpoint (optional) */
                json_t* endpointJSON = json_object_get(peerBlock, "endpoint");
                if(endpointJSON)
                {
                    /* Fetch the peer.endpoint */
                    uint8_t* endpoint = json_string_value(endpointJSON);
                    if(endpoint)
                    {
                        printf("Endpoint: %s\n", endpoint);
                        newSession->peer.endpoint = endpoint;
                    }
                    else
                    {
                        printf("`endpoint` must be a string\n");
                        return 0;
                    }
                  
                    /* Fetch peer.bind (optional) */
                    json_t* bindJSON = json_object_get(peerBlock, "bind");
                    if(bindJSON)
                    {
                        /* Fetch the peer.bind */
                        uint8_t* bind = json_string_value(bindJSON);
                        if(bind)
                        {
                            printf("Bind (for source): %s\n", bind);
                            newSession->peer.bindAddr = bind;
                        }
                        else
                        {
                            printf("`bind` must be a string\n");
                            return 0;
                        }
                    }
                }
                
                /**
                 * TODO: Later features to add near end of the alpha "alpha"
                 * will be the setting up of hooks
                 */
            }
            else
            {
                printf("Could not find peer block for session '%s'\n", key);
                return 0;
            }
        }
        else
        {
            printf("Allocation failure\n");
            return 0;
        }
    }

    return head;
}
