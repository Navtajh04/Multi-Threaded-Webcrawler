#ifndef PASTER_INCLUDE_CURL_CALLBACKS_H
#define PASTER_INCLUDE_CURL_CALLBACKS_H

#include <stdio.h>

size_t curlHeaderCallback(char *p_recv, size_t size, size_t nmemb, void *userdata);

/**
 * @brief write callback function to save a copy of received data in RAM.
 *        The received libcurl data are pointed by p_recv, 
 *        which is provided by libcurl and is not user allocated memory.
 *        The user allocated memory is at p_userdata. One needs to
 *        cast it to the proper struct to make good use of it.
 *        This function maybe invoked more than once by one invokation of
 *        curl_easy_perform().
 */

size_t curlWriteCallback(char *p_recv, size_t size, size_t nmemb, void *p_userdata);


#endif /* PASTER_INCLUDE_CURL_CALLBACKS_H */
