#include <stdafx.h>
#include "FullscreenQuad.h"
#include "Buffer.h"
#include "Program.h"
#include "math3d.h"
#include <vector>
#include "glfuncs.h"
#include "Buffer.h"

FullscreenQuad::FullscreenQuad()
{
    std::vector<float> pos( {-1,1,-1,  -1,-1,-1,  1,1,-1,  1,-1,-1 } );
    std::vector<float> tex( {0,1,  0,0,   1,1,  1,0} );
    std::vector<std::uint32_t> indices( {0,1,2,  2,1,3} );
        
    Buffer posbuff(pos);
    Buffer texbuff(tex);
    Buffer indexbuff(indices);
    
    GLuint tmp[1];
    glGenVertexArrays(1,tmp);
    this->vao = tmp[0];
    glBindVertexArray(vao);
    indexbuff.bind(GL_ELEMENT_ARRAY_BUFFER);
    posbuff.bind(GL_ARRAY_BUFFER);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 0, 3, GL_FLOAT, false, 3*4, 0 );
        
    texbuff.bind(GL_ARRAY_BUFFER);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer( 1, 2, GL_FLOAT, false, 2*4, 0 );

    glBindVertexArray(0);
}
    
void FullscreenQuad::draw(){
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((char*)0) );
}

FullscreenQuad::~FullscreenQuad(){
    glDeleteVertexArrays(1,&this->vao);
}
