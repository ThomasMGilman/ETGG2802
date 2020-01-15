#include "glfuncs.h"
#include "Buffer.h"
#include <iostream>

Buffer::Buffer( void* p, size_t size, GLenum usage){
    GLuint tmp[1];
    glGenBuffers(1,tmp);
    buffID = tmp[0];
    glBindBuffer( GL_ARRAY_BUFFER, this->buffID );
    glBufferData(GL_ARRAY_BUFFER, size, p, usage);
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Buffer::bind(GLenum bindingPoint){
    glBindBuffer(bindingPoint, this->buffID);
}

void Buffer::bindBase(GLenum bindingPoint, unsigned index){
    glBindBufferBase(bindingPoint, index, this->buffID);
}

Buffer::~Buffer(){
    glDeleteBuffers(1,&this->buffID);
}
