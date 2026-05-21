#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");

  if (!file) {
    fprintf(stderr, "❌Failed to open file: [%s]\n", path);
    exit(EXIT_FAILURE);
    return NULL;
  }

  // Check length
  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  if (length < 0) {
    fprintf(stderr, "⚠️Nothing to read from file: [%s]\n", path);
    fclose(file);
    return NULL;
  }

  // Back to the beginning of the file
  fseek(file, 0, SEEK_SET);

  char* buf = (char*)malloc(length + 1); // +1 for the null terminator '\0'
  if (!buf) {
    fprintf(stderr, "❌Memory allocation failed for file: [%s]\n", path);
    exit(EXIT_FAILURE);
    return NULL;
  }

  size_t bytesRead = fread(buf, 1, length, file);
  buf[bytesRead] = '\0';

  fclose(file);

  return buf;
}

