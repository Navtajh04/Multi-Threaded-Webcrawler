#include "../include/curl_callbacks.h"
#include "../include/curl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>

#define max(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/**
 * @brief callback function to save the sequence number of a received PNG fragment
 * 
 * @param p_recv the received libcurl data
 * @param size size in bytes of each data element being received
 * @param nmemb number of data elements being received
 * @param p_userdata buffer to store the received data
 * 
 * @return size_t - number of bytes that were received by libcurl
*/
size_t curlHeaderCallback(char *p_recv, size_t size, size_t nmemb, void *userdata) {
    int realsize = size * nmemb;
    curl_recv_buf_t *p = userdata;
    
    if (realsize > strlen(ECE252_HEADER) &&
	strncmp(p_recv, ECE252_HEADER, strlen(ECE252_HEADER)) == 0) {

        // extract img sequence number
	    p->seq = atoi(p_recv + strlen(ECE252_HEADER));

    }
    return realsize;
}

/**
 * @brief callback function to save received data from cURL to a buffer
 * 
 * @param p_recv the received libcurl data
 * @param size size in bytes of each data element being received
 * @param nmemb number of data elements being received
 * @param p_userdata pointer to struct to store the received data
 * 
 * @return size_t - number of bytes that were received by libcurl
*/
size_t curlWriteCallback(char *p_recv, size_t size, size_t nmemb, void *p_userdata) {
    size_t realsize = size * nmemb;
    curl_recv_buf_t *p = (curl_recv_buf_t *)p_userdata;
 
    if (p->size + realsize + 1 > p->maxSize) {/* hope this rarely happens */ 
        /* received data is not 0 terminated, add one byte for terminating 0 */
        size_t new_size = p->maxSize + max(BUF_INC, realsize + 1);   
        char *q = realloc(p->buf, new_size);
        if (q == NULL) {
            perror("realloc"); /* out of memory */
            return -1;
        }
        p->buf = q;
        p->maxSize = new_size;
    }

    memcpy(p->buf + p->size, p_recv, realsize); /*copy data from libcurl*/
    p->size += realsize;
    p->buf[p->size] = 0;

    return realsize;
}
