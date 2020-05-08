#pragma once
#include "glfuncs.h"

class Texture{
    Texture(const Texture&) = delete;
    void operator=(const Texture&) = delete;
    
  protected:
    friend class Framebuffer;
    GLuint type;
    GLuint tex;
  
  public:
    
    /** Constructor. This sets this->type and initializes this->tex to zero*/
    Texture(GLuint typ);
    
    virtual ~Texture();
    
    /** Bind this texture to the given texture unit.*/
    void bind(GLuint unit);
    
    /** Binds 0 to the given texture unit.*/
    void unbind(GLuint unit);
};
 
