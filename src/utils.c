#include "utils.h"

#include <cglm/types-struct.h>
#include <stdio.h>
#include <stdlib.h>

#define READ_CHARS_LENGTH 0xff

char* readFile(const char* path, size_t* outSize) {
  FILE* fptr;
  fopen_s(&fptr, path, "r");

  size_t sz = sizeof(char) * READ_CHARS_LENGTH;
  char* buf = malloc(sz);

  if (fptr) {
    u32 idx = 0;
    while (!feof(fptr)) {
      if (idx == sz / sizeof(char)) {
        char* newBuf = malloc(sz * 2);
        for (u32 i = 0; i < idx; i++)
          newBuf[i] = buf[i];
        free(buf);
        sz *= 2;
        buf = newBuf;
      }
      buf[idx++] = fgetc(fptr);
    }

    // Cut extra memory
    char* newBuf = malloc(sizeof(char) * idx);
    for (u32 i = 0; i < idx; i++)
      newBuf[i] = buf[i];
    free(buf);
    sz = sizeof(char) * idx;
    buf = newBuf;

    if (outSize)
      *outSize = sz;
  } else {
    printf("Not able to open the file [%s]\n", path);
    free(buf);
  }

  return buf;

  fclose(fptr);
}

vec3s hexToVec3sColor(u32 hex) {
  return (vec3s) {
    (hex >> 16) / 255.f,
    ((hex >> 8) & 0xff) / 255.f,
    (hex & 0xff) / 255.f
  };
}

