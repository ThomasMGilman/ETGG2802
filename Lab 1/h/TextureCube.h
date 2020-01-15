#pragma once

#include "Texture.h"

class TextureCube : public Texture {
  public:
    unsigned size;
    
    /** Initializes the cube texture. Cube textures must have equal
        width, height, and depth. */
    TextureCube(unsigned size);
};
