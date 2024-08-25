#ifndef UTILS_H
#define UTILS_H

/**
 * @param path A path to a file
 * @param outSize The size of the filled return array (not required)
 * @return Returns dynamic array of characters (needs to be freed)
 */
char* readFile(const char* path, size_t* outSize);

#endif

