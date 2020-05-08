#include <stdafx.h>
#include "Texture.h"

Texture::Texture(GLuint typ){
    this->type = typ;
    this->tex = 0;
}

void Texture::bind(GLuint unit){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(this->type,this->tex);
}

void Texture::unbind(GLuint unit){
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(this->type,0);
}

Texture::~Texture(){
    glDeleteTextures(1,&this->tex);
}
