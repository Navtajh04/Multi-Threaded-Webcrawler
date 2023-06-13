#ifndef PASTER_INCLUDE_CURL_H
#define PASTER_INCLUDE_CURL_H

#include <stdio.h>

#define IMG_URL_BASE "http://ece252-1.uwaterloo.ca:2520/image?img="
#define DUM_URL "https://example.com/"
#define ECE252_HEADER "X-Ece252-Fragment: "
#define BUF_SIZE 1048576  /* 1024*1024 = 1M */
#define BUF_INC  524288   /* 1024*512  = 0.5M */

typedef struct {
    char *buf;       /* memory to hold a copy of received data */
    size_t size;     /* size of valid data in buf in bytes*/
    size_t maxSize; /* max capacity of buf in bytes*/
    int seq;         /* >=0 sequence number extracted from http header */
                     /* <0 indicates an invalid seq number */
} curl_recv_buf_t;

/**
 * @brief initialize the recv buf struct to be used to store data received from libcurl
 * 
 * @param ptr pointer to a curl_recv_buf_t struct to be initialized
 * @param maxSize max size of the received data
 * 
 * @return int - 0 if successful
*/
int curlRecvBufInit(curl_recv_buf_t *ptr, size_t maxSize);

/**
 * @brief clean up the memory allocated to the curl_recv_buf_t struct
 * 
 * @param ptr pointer to the curl_recv_buf_t struct that needs to be cleaned up
 * 
 * @return int - 0 if successful
*/
int curlRecvBufCleanup(curl_recv_buf_t *ptr);

/**
 * @brief get the PNG data from the specified web server
 * 
 * @param recvBuf buffer to store the received PNG data
 * @param imageNum which web server to get PNG data from
 * 
 * @return int - 0 if successful
*/
int curlGetData(curl_recv_buf_t *recvBuf, int imageNum);

#endif /* PASTER_INCLUDE_CURL_H */
