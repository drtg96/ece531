#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdbool.h>

#define OK       0
#define INIT_ERR 1
#define REQ_ERR  2

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
	\n\t./hw -d -u <web_url> message\
	\n\t./hw -h\
	\n<web_url>:\t http://www.website.com:port\n");
}

bool isValidateURL(cosnt char* url)
{
	// Check if the URL starts with "http://"
	if (strstr(url, "http://") != NULL) 
	{
		if (strstr(url, "localhost") != NULL)
		{
	        // Check if a port number is specified
	        const char* portStart = strchr(url, ':');
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
	    // Just have to assume the web address exists at this point
        return true;
    }
    return false;
}

static int send_http_requeset(char *url, char *type, char *message, bool verb)
{
	CURL *curl = curl_easy_init();
    if (curl)
    {
 		CURLcode res;
       	curl_easy_setopt(curl, CURLOPT_URL, url);
       	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, type);
       	if (verb)
       	{
       		printf("sending message: %s\n", message);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        } 
        else
        {
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        }
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            return REQ_ERR;
        }

        curl_easy_cleanup(curl);
    }
    else
    {
        return INIT_ERR;
    }
    return OK;
}

char * getVerb(int argc, char *argv[])
{
	int c;
	int message_len = 0;
	for(c=3; c < argc; c++)
	{
		message_len += strlen(argv[c]) + 1;
	}
	char * curl_message[message_len];
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
	return curl_message;
}

int main(int argc, char *argv[])
{
	int err;	
	if(argc < 2 || 
		(strcmp(argv[1], "--help") == 0) ||
	 	(strcmp(argv[1], "-h") == 0))
	{
		usage();
		return OK;
	}
	
	if((strcmp(argv[1], "--get") == 0) || (strcmp(argv[1], "-g") == 0))
	{
		if((strcmp(argv[2], "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
                err = send_http_requeset(argv[3], "GET", NULL, false);
			}
			else
			{
				usage();
				return REQ_ERR;
			}
		}
	}
	else if((strcmp(argv[1], "--post") == 0) || (strcmp(argv[1], "-o") == 0))
	{
		if((strcmp(argv[2],  "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
				char * curl_message = getVerb(argc, argv);
				err = send_http_requeset(argv[3], "POST", curl_message, true);
			}
			usage();
			return REQ_ERR;
		}
	}
	else if((strcmp(argv[1], "--put") == 0) || (strcmp(argv[1], "-p") == 0))
	{
		if((strcmp(argv[2],  "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
				char * curl_message = getVerb(argc, argv);
				err = send_http_requeset(argv[3], "PUT", curl_message, true);
			}
			usage();
			return REQ_ERR;
		}
	}
	else if((strcmp(argv[1], "--delete") == 0) || (strcmp(argv[1], "-d") == 0))
	{
		if((strcmp(argv[2],  "--url") == 0) || (strcmp(argv[2], "-u") == 0))
		{
			if(isValidateURL(argv[3]))
			{
				char * curl_message = getVerb(argc, argv);
				err = send_http_requeset(argv[3], "DELETE", curl_message, true);
			}
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
				char * curl_message = getVerb(argc, argv);
				err = send_http_requeset(argv[3], "DELETE", curl_message, true);
			}
			else if((strcmp(argv[3], "--put") == 0) || (strcmp(argv[3], "-p") == 0))
			{
				char * curl_message = getVerb(argc, argv);
				err = send_http_requeset(argv[3], "PUT", curl_message, true);
			}
			else if((strcmp(argv[3], "--post") == 0) || (strcmp(argv[3], "-o") == 0))
			{
				char * curl_message = getVerb(argc, argv);
				err = send_http_requeset(argv[3], "POST", curl_message, true);
			}
			else if((strcmp(argv[3], "--get") == 0) || (strcmp(argv[3], "-g") == 0))
			{               
				err = send_http_requeset(argv[3], "GET", NULL, false);
			}
			else
			{
				usage();
				return REQ_ERR;
			}
		}
	}
	else
	{
		usage();
		return OK;
	}
	return err;
}
