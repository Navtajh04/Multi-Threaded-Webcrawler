
#include "../include/paster.h"
#include "../include/curl_callbacks.h"
#include "../include/png_util.h"
#include "../include/curl.h"
#include "../include/zutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <zlib.h>
#include <curl/curl.h>
#include <pthread.h>

static uint8_t fragmentsReceived[NUMBER_OF_FRAGEMENTS];
static uint8_t uniqueFragmentsReceived;
static pthread_mutex_t fragmentReceivedCheckMutex;
static pthread_mutex_t pthreadArgMutex;

static void *getUncompressedDataCallback(void *pthreadArgPtr);

int getImage(int numThreads, int imageNum){
    pthread_arg_t pthreadArg = {0};
    pthread_t *threadHandles = malloc(sizeof(pthread_t) * numThreads);
    uint8_t ret[numThreads];
    pthread_mutex_init(&fragmentReceivedCheckMutex, NULL);
    pthread_mutex_init(&pthreadArgMutex, NULL);
    pthreadArg.uncompressedData.data = malloc(PNG_HEIGHT*((PNG_WIDTH*4) + 1));
    pthreadArg.imageNum = imageNum;
     
    for (int i=0; i<numThreads; i++) {
        pthread_create(threadHandles + i, NULL, getUncompressedDataCallback, (void*) &pthreadArg); 
    }

    for (int i=0; i<numThreads; i++) {
        pthread_join(threadHandles[i], (void **)&(ret[i]));
    }

    uint8_t *idatCompressedData = malloc(pthreadArg.uncompressedData.length);
    uint64_t idatCompressedDataLen = 0;
    memDef(idatCompressedData, &idatCompressedDataLen, pthreadArg.uncompressedData.data, pthreadArg.uncompressedData.length, Z_DEFAULT_COMPRESSION);
    if(createPNGFile(PNG_WIDTH, PNG_HEIGHT, idatCompressedData, idatCompressedDataLen) != 0){
        printf("Could not create the PNG file\n");
        return 1;
    }
    /* cleaning up */

    free(threadHandles);
    
    /* the memory was allocated in the do_work thread for return values */
    /* we need to free the memory here */

    return 0;
}

static void *getUncompressedDataCallback(void *pthreadArgPtr){
    pthread_arg_t *pthreadArg = (pthread_arg_t *) pthreadArgPtr;
    while(uniqueFragmentsReceived < 50){
        curl_recv_buf_t recvBuf;
        curlGetIdatData(&recvBuf, pthreadArg->imageNum);
        pthread_mutex_lock(&fragmentReceivedCheckMutex);
        if(fragmentsReceived[recvBuf.seq] == 1){
            pthread_mutex_unlock(&fragmentReceivedCheckMutex);
            continue;
        }
        fragmentsReceived[recvBuf.seq] = 1;
        pthread_mutex_unlock(&fragmentReceivedCheckMutex);
        uncompressed_data_t idatUncompressedData = {0};
        idatUncompressedData.data = malloc(PNG_HEIGHT*((PNG_WIDTH*4) + 1));
        getIdatData(&idatUncompressedData, (uint8_t*) recvBuf.buf);
        memcpy(pthreadArg->uncompressedData.data + idatUncompressedData.length*recvBuf.seq, idatUncompressedData.data, idatUncompressedData.length);
        pthread_mutex_lock(&pthreadArgMutex);
        pthreadArg->uncompressedData.length += idatUncompressedData.length;
        pthread_mutex_unlock(&pthreadArgMutex);
        curlRecvBufCleanup(&recvBuf);
        free(idatUncompressedData.data);
        uniqueFragmentsReceived++;
    }
    pthread_exit(0);
}