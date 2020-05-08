#pragma once
#include "Texture.h"

class Texture2DArray: public Texture {
    Texture2DArray(const Texture2DArray&) = delete;
    void operator=(const Texture2DArray&) = delete;

  public:
    unsigned w,h,slices;
    Texture2DArray(unsigned w, unsigned h, unsigned slices);
    void generateMipmap();
};
