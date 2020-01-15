
#include "TextureCube.h"

TextureCube::TextureCube(unsigned size) : Texture( GL_TEXTURE_CUBE_MAP ){
    this->size=size;
}
