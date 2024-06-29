/******************************************************************************
* INSTRUCTIONS
* Assignment: Network Communication on ARM
* use make -f makefile-arm to build for qemu-arm image on buildroot-2024.02.2
* output progam is called `client`
* to run see program see usage with `./client -h`
* NOTE that attention needs to be paid to network configuration. i.e. It is
* recommended to use verbose ip addresses instead of localhost between UNIX real
* and virtual machines
******************************************************************************/

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdbool.h>

#define OK       0
#define INIT_ERR 1
#define REQ_ERR  2

#define VERB_LEN  256
#define URL "http://localhost:8000"

/******************************************************************************
* Methods
******************************************************************************/

// HELP message (-h / --help)
void usage()
{
 	printf("Usage:\
	\n-u/--url\tTakes a properly formated URL (including localhost) followed by a port number\
	\n-o/--post\tPerform an HTTP POST\
	\n-g/--get\tPerform an HTTP GET\
	\n-p/--put\tPerform an HTTP PUT\
	\n-d/--delete\tPerform an HTTP DELETE\
	\n-h/--help\tShow this usage message\
	\n\nExamples:\
	\n\t./client -d -u <web_url> message\
	\n\t./client --url <web_url> -g\
	\n<web_url>:\t http://www.website.com:<optional port>\
	\n<web_url>:\t http://localhost:<port>\n");
}

// Some basic checks on the web url
bool isValidateURL(const char* url)
{
	// Check if the URL starts with "http://"
	if (strstr(url, "http://") != NULL) 
	{
		const char* localhost = strstr(url, "localhost");
		if (localhost != NULL)
		{
	        // Check if a port number is specified on our traversed string
	        const char* portStart = strchr(localhost, ':');
	        if (portStart)
	        {        		
    	        int portNumber;
    	        if (sscanf(portStart + 1, "%d", &portNumber) == 1)
    	        {
    	            return true;
    	        }
    	    }
    	    return false;
	    }
	    // Just have to assume the web address exists at this point.
        return true;
    }
    return false;
}

// request handler
static int send_HTTP_Request(char* url, char* type, char* message, bool verb)
{
	CURL* curl = curl_easy_init();
    if (curl)
    {
    	long http_code = 0;
 		CURLcode res;
       	curl_easy_setopt(curl, CURLOPT_URL, url);
       	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, type);
       	if (verb)
       	{
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        } 
        else
        {
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        }
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
			fprintf(stderr, "curl could not execute --get: %s\n", curl_easy_strerror(res));
            return REQ_ERR;
        }
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		printf("\nHTTP CODE: %ld\n", http_code);

        curl_easy_cleanup(curl);
    }
    else
    {
        return INIT_ERR;
    }
    return OK;
}

// Helper to extract arbitrary length verbs
void getVerb(int argc, char *argv[], char* curl_message)
{
	// this argument is always the final argument
	int c = 3;
	
	// Calculate the total length needed for the concatenated string
	int message_len = 0;
	for(c=3; c < argc; c++)
	{
		message_len += strlen(argv[c]) + 1;
	}
    
    if (message_len > VERB_LEN)
    {
    	printf("Message length is %d, Needs to be less than %d\n", message_len, VERB_LEN);
    	return;
    }
	
	for(c=4; c < argc; c++)
	{
		strcat(curl_message, argv[c]);
		if((c + 1) == argc)
		{	
			;
		}
		else
		{
			strcat(curl_message, " ");
		}
	}
}

int main(int argc, char *argv[])
{
    /*printf("argc: %d\nargv: ", argc);
    for(int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");*/
	int err;	
	if(argc < 4 || 
		(strcmp(argv[1], "--help") == 0) ||
	 	(strcmp(argv[1], "-h") == 0))
	{
		usage();
		return OK;
	}
	
	char curl_message[256];
	
	if((strcmp(argv[1], "--get") == 0) || (strcmp(argv[1], "-g") == 0))
	{
		if((strcmp(argv[2], "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
			    //printf("\nTest valide URL\n");
                err = send_HTTP_Request(argv[3], "GET", NULL, false);
			}
			else
			{
				usage();
				return REQ_ERR;
			}		
		}
		else
		{
			usage();
			return REQ_ERR;
		}
	}
	else if((strcmp(argv[1], "--post") == 0) || (strcmp(argv[1], "-o") == 0))
	{
		if((strcmp(argv[2], "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
				getVerb(argc, argv, curl_message);
				if(!curl_message)
				{
					return REQ_ERR;
				}
				err = send_HTTP_Request(argv[3], "POST", curl_message, true);
			}
			else
			{
				usage();
				return REQ_ERR;
			}
		}
		else
		{
			usage();
			return REQ_ERR;
		}
	}
	else if((strcmp(argv[1], "--put") == 0) || (strcmp(argv[1], "-p") == 0))
	{
		if((strcmp(argv[2], "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
				getVerb(argc, argv, curl_message);
				if(!curl_message)
				{
					return REQ_ERR;
				}
				err = send_HTTP_Request(argv[3], "PUT", curl_message, true);
			}
			else
			{
				usage();
				return REQ_ERR;
			}		
		}
		else
			{
				usage();
				return REQ_ERR;
			}
	}
	else if((strcmp(argv[1], "--delete") == 0) || (strcmp(argv[1], "-d") == 0))
	{
		if((strcmp(argv[2], "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
				getVerb(argc, argv, curl_message);
				//printf("Test getVerb(): %s\n", curl_message);
				if(!curl_message)
				{
					return REQ_ERR;
				}
				err = send_HTTP_Request(argv[3], "DELETE", curl_message, true);
			}
			else
			{
				usage();
				return REQ_ERR;
			}		
		}
		else
			{
				usage();
				return REQ_ERR;
			}
	}
	// Allows for args to be handled in any order
	else if((strcmp(argv[1], "--url") == 0) || (strcmp(argv[1], "-u") == 0))
	{
		if(isValidateURL(argv[2]))
		{
			if((strcmp(argv[3], "--delete") == 0) || (strcmp(argv[3], "-d") == 0))
			{
				getVerb(argc, argv, curl_message);
				if(!curl_message)
				{
					usage();
					return REQ_ERR;
				}
				err = send_HTTP_Request(argv[2], "DELETE", curl_message, true);
			}
			else if((strcmp(argv[3], "--put") == 0) || (strcmp(argv[3], "-p") == 0))
			{
				getVerb(argc, argv, curl_message);
				if(!curl_message)
				{
					usage();
					return REQ_ERR;
				}
				err = send_HTTP_Request(argv[2], "PUT", curl_message, true);
			}
			else if((strcmp(argv[3], "--post") == 0) || (strcmp(argv[3], "-o") == 0))
			{
				getVerb(argc, argv, curl_message);
				if(!curl_message)
				{
					usage();
					return REQ_ERR;
				}
				err = send_HTTP_Request(argv[2], "POST", curl_message, true);
			}
			else if((strcmp(argv[3], "--get") == 0) || (strcmp(argv[3], "-g") == 0))
			{               
				err = send_HTTP_Request(argv[2], "GET", NULL, false);
			}
			else
			{
				usage();
				return REQ_ERR;
			}
		}
		else	
		{
			usage();
			return REQ_ERR;
		}
	}
	else
	{
		usage();
		return OK;
	}
	
	if(err != 0)
	{
		usage();
	}
	return err;
}

