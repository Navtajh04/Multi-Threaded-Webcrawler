
#include "../include/paster.h"
#include "../include/curl_callbacks.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <curl/curl.h>
#include <pthread.h>

#define NUMBER_OF_FRAGEMENTS 50
#define PNG_HEIGHT 300
#define PNG_WIDTH 400
#define PNG_FRAGMENT_HEIGHT 6

static uint8_t fragmentsReceived[NUMBER_OF_FRAGEMENTS];

int getImage(int numThreads, int imageNum){
    pthread_t *p_tids = malloc(sizeof(pthread_t) * numThreads);
    struct thread_args in_params[numThreads];
    struct thread_ret *p_results[numThreads];
     
    for (int i=0; i<numThreads; i++) {
        in_params[i].x = i + 1;
        in_params[i].y = i + 2;
        pthread_create(p_tids + i, NULL, do_work, in_params + i); 
    }

    for (int i=0; i<numThreads; i++) {
        pthread_join(p_tids[i], (void **)&(p_results[i]));
        printf("Thread ID %lu joined.\n", p_tids[i]);
        printf("sum(%d,%d) = %d.\n", \
               in_params[i].x, in_params[i].y, p_results[i]->sum); 
        printf("product(%d,%d) = %d.\n\n", \
               in_params[i].x, in_params[i].y, p_results[i]->product); 
    }

    /* cleaning up */

    free(p_tids);
    
    /* the memory was allocated in the do_work thread for return values */
    /* we need to free the memory here */
    for (uint8_t i=0; i < numThreads; i++) {
        free(p_results[i]);
    }

    return 0;
}