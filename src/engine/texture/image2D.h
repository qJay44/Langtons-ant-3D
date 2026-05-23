#ifndef IMAGE_2D_H
#define IMAGE_2D_H

typedef struct {
  int width, height, channels;
  void* pixels;
} image2D;

image2D image2D_load(const char* filepath, bool flipVertically);

#endif

