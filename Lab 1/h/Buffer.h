#pragma once 

#include "glfuncs.h"
#include <vector>
#include <array>

class Buffer{
  private:
    Buffer(const Buffer&) = delete;
    void operator=(const Buffer&) = delete;
    
    GLuint buffID;
    Buffer( void* p, size_t size, GLenum usage);
  public:
    
    template<typename T>
    Buffer(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW) : 
        Buffer( data.data(), data.size(), usage) 
    {
    }

    template<typename T, unsigned n>
    Buffer(const std::array<T,n>& data, GLenum usage=GL_STATIC_DRAW) : 
        Buffer( data.data(), sizeof(T)*data.size(), usage) 
    {
    }
    
    template<typename T>
    Buffer(const T* p, size_t count, GLenum usage=GL_STATIC_DRAW): 
        Buffer( (void*) p, count*sizeof(T), usage) 
    {
    }
    
    Buffer( size_t size, GLenum usage=GL_STATIC_DRAW): 
        Buffer( nullptr, size, usage) 
    {
    }
    
    ~Buffer();
    
    void bind(GLenum bindingPoint);
    void bindBase(GLenum bindingPoint, unsigned index);
};

