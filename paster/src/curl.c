#include "../include/curl.h"
#include "../include/curl_callbacks.h"
#include "../include/png_util.h"
#include "../include/paster.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>
#include <string.h>

#define IMAGE_NUM_DIGITS 1
#define URL_MAX_SIZE 256

/**
 * @brief initialize the recv buf struct to be used to store data received from libcurl
 * 
 * @param ptr pointer to a curl_recv_buf_t struct to be initialized
 * @param maxSize max size of the received data
 * 
 * @return int - 0 if successful
*/
int curlRecvBufInit(curl_recv_buf_t *ptr, size_t maxSize) {
    void *p = NULL;
    
    if (ptr == NULL) {
        return 1;
    }

    p = malloc(maxSize);
    if (p == NULL) {
	return 2;
    }
    
    ptr->buf = p;
    ptr->size = 0;
    ptr->maxSize = maxSize;
    ptr->seq = -1;              /* valid seq should be non-negative */
    return 0;
}

/**
 * @brief clean up the memory allocated to the curl_recv_buf_t struct
 * 
 * @param ptr pointer to the curl_recv_buf_t struct that needs to be cleaned up
 * 
 * @return int - 0 if successful
*/
int curlRecvBufCleanup(curl_recv_buf_t *ptr) {
    if (ptr == NULL) {
	return 1;
    }
    
    free(ptr->buf);
    ptr->size = 0;
    ptr->maxSize = 0;
    return 0;
}

/**
 * @brief get the PNG data from the specified web server
 * 
 * @param recvBuf buffer to store the received PNG data
 * @param imageNum which web server to get PNG data from
 * 
 * @return int - 0 if successful
*/
int curlGetData(curl_recv_buf_t *recvBuf, int imageNum){
    CURL *curlHandle;
    CURLcode res;
    char url[URL_MAX_SIZE] = IMG_URL_BASE;
    char fname[URL_MAX_SIZE];
    pid_t pid =getpid();
    
    curlRecvBufInit(recvBuf, BUF_SIZE);

    char imageNumStr[IMAGE_NUM_DIGITS + 1];
    sprintf(imageNumStr, "%d", imageNum);
    strcat(url, imageNumStr);
    printf("URL is %s\n", url);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init a curl session */
    curlHandle = curl_easy_init();

    if (curlHandle == NULL) {
        fprintf(stderr, "curl_easy_init: returned NULL\n");
        return 1;
    }

    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    /* register write call back function to process received data */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, curlWriteCallback); 
    /* user defined data structure passed to the call back function */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *) recvBuf);

    /* register header call back function to process received header data */
    curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, curlHeaderCallback); 
    /* user defined data structure passed to the call back function */
    curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, (void *) recvBuf);

    /* some servers requires a user-agent field */
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    
    /* get it! */
    res = curl_easy_perform(curlHandle);

    if( res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return 1;
    } else {
	printf("%lu bytes received in memory %p, seq=%d.\n", \
               recvBuf->size, recvBuf->buf, recvBuf->seq);
    }

    sprintf(fname, "./output_%d_%d.png", recvBuf->seq, pid);

    /* cleaning up */
    curl_easy_cleanup(curlHandle);
    curl_global_cleanup();
    return 0;
}
