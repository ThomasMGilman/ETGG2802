#include <stdafx.h>
#include "DataTexture2DArray.h"


DataTexture2DArray::DataTexture2DArray(unsigned w, unsigned h, unsigned slices, GLenum ifmt, GLenum efmt, GLenum etype, const void* pix) : 
    Texture2DArray(w,h,slices) 
{
    GLuint tmp[1];
    glGenTextures(1,tmp);
    this->tex = tmp[0];
    this->ifmt = ifmt;
    this->bind(0);
    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, ifmt,
        w,h,slices, 0, efmt, 
        etype, pix );
    this->unbind(0);
}



void DataTexture2DArray::setData(unsigned w, unsigned h, unsigned slices, GLenum ifmt, GLenum efmt, GLenum etype, const void* pix)
{
    this->bind(0);
    if( w == this->w && h == this->h && slices == this->slices && ifmt == this->ifmt ){
        glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0,0,0, w,h,slices, efmt, etype, pix );
    } else{
        this->ifmt = ifmt;
        this->w=w;
        this->h=h;
        this->slices=slices;
        glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, ifmt,
            w,h,slices, 0, efmt, 
            etype, pix );
    }
    this->unbind(0);
}

