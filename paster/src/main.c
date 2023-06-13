#include "../include/paster.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DECIMAL_NUM_BASE 10

/**
 * @brief: a command that takes -t and -n options
 * -t <number of threads>, default value 1 (i.e. single threaded)
 * -n <image number>, default value 1 (valid values: 1, 2, and 3)
 *    There are three images in total on the server side.
 * SYNOPSIS
 *        <command> [-t <number of threads>] [-n <image number>]
 */

int main(int argc, char **argv) {
    int c;
    int numThreads = 1;
    int imageNum = 1;
    char *str = "option requires an argument";
    
    while ((c = getopt (argc, argv, "t:n:")) != -1) {
        switch (c) {
        case 't':
	    numThreads = strtoul(optarg, NULL, DECIMAL_NUM_BASE);
	    printf("option -t specifies a value of %d.\n", numThreads);
	    if (numThreads <= 0 || numThreads > __UINT8_MAX__) {
                fprintf(stderr, "%s: 0 < %s < 255 -- 't'\n", argv[0], str);
                return -1;
            }
            break;
        case 'n':
            imageNum = strtoul(optarg, NULL, DECIMAL_NUM_BASE);
	    printf("option -n specifies a value of %d.\n",  imageNum);
            if ( imageNum <= 0 ||  imageNum > 3) {
                fprintf(stderr, "%s: %s 1, 2, or 3 -- 'n'\n", argv[0], str);
                return -1;
            }
            break;
        default:
            return -1;
        }
    }
    getImage(numThreads, imageNum);
}
