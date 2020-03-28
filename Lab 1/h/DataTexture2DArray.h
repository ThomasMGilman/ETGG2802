#pragma once 

#include "Texture2DArray.h"

class DataTexture2DArray: public Texture2DArray{
    GLenum ifmt;
  public:
    DataTexture2DArray(unsigned w, unsigned h, unsigned slices, GLenum ifmt, GLenum efmt, GLenum etype, const void* pix);
    void setData(unsigned w, unsigned h, unsigned slices, GLenum ifmt, GLenum efmt, GLenum etype, const void* pix);
};
