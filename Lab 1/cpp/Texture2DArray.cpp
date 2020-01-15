
#include "Texture2DArray.h"

Texture2DArray::Texture2DArray(unsigned w, unsigned h, unsigned slices) :
    Texture(GL_TEXTURE_2D_ARRAY)
{
    this->w=w;
    this->h=h;
    this->slices=slices;
}

void Texture2DArray::generateMipmap(){
    this->bind(0);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}
