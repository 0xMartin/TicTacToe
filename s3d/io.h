/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    io.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef IO_H
#define IO_H

#include <stdbool.h>

#define DEFAULT_LOG_ERR_FILE "error.txt"

/**
 * @brief IO_readStringFromFile
 * @param path
 * @param buffer
 * @param size
 * @return
 */
bool IO_readStringFromFile(const char * const path, char * buffer, unsigned int size);

/**
 * @brief IO_writeStringToFile
 * @param path
 * @param buffer
 * @return
 */
bool IO_writeStringToFile(const char * const path, char * buffer);

/**
 * @brief IO_appendStringToFile
 * @param path
 * @param buffer
 * @return
 */
bool IO_appendStringToFile(const char * const path, char * buffer);

/**
 * @brief IO_readDataFromFile
 * @param path
 * @param data
 * @return
 */
bool IO_readDataFromFile(const char * const path, void * data);

/**
 * @brief IO_writeDataToFile
 * @param path
 * @param data
 * @return
 */
bool IO_writeDataToFile(const char * const path, void * data);

/**
 * @brief IO_writeDataToFile
 * @param path
 * @param data
 * @return
 */
bool IO_logError(const char * const str);


#endif // IO_H
