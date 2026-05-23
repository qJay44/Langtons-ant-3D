#include "image2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

image2D image2D_load(const char* filepath, bool flipVertically) {
  image2D img = {0};
  stbi_set_flip_vertically_on_load(flipVertically);
  img.pixels = stbi_load(filepath, &img.width, &img.height, &img.channels, 0);

  return img;
}

