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
    size_t max_size; /* max capacity of buf in bytes*/
    int seq;         /* >=0 sequence number extracted from http header */
                     /* <0 indicates an invalid seq number */
} curl_recv_buf_t;

int curlRecvBufInit(curl_recv_buf_t *ptr, size_t max_size);

int curlRecvBufCleanup(curl_recv_buf_t *ptr);

int curlGetIdatData(curl_recv_buf_t *recvBuf, int imageNum);

#endif /* PASTER_INCLUDE_CURL_H */