#include "Image.h"
#include <stdexcept>
#include <vector>
#include "ImageTextureCube.h"
#include <cstdio>

    
ImageTextureCube::ImageTextureCube( std::string namepattern ) : TextureCube(0) {
    GLuint tmp[1];
    glGenTextures(1,tmp);
    this->tex = tmp[0];
    this->bind(0);
    for(int i=0;i<6;++i){
        size_t sz = snprintf(nullptr,0,namepattern.c_str(),i);
        std::vector<char> buff(sz+1);
        snprintf(buff.data(),buff.size(),namepattern.c_str(),i);
        std::string fname = "assets/";
        fname += buff.data();
        Image img(fname);
        
        if(img.width() != img.height() )
            throw std::runtime_error("Cubemap must be square: " + fname );
        if(i == 0)
            this->size = img.width();
        else{
            if(this->size != img.width() )
                throw std::runtime_error("Cubemap sides must be the same size (file "+fname+")");
        }
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGBA8, this->size, this->size, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, img.pixels() );
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
