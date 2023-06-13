#ifndef PASTER_INCLUDE_CURL_CALLBACKS_H
#define PASTER_INCLUDE_CURL_CALLBACKS_H

#include <stdio.h>

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
size_t curlHeaderCallback(char *p_recv, size_t size, size_t nmemb, void *userdata);

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
size_t curlWriteCallback(char *p_recv, size_t size, size_t nmemb, void *p_userdata);


#endif /* PASTER_INCLUDE_CURL_CALLBACKS_H */
