
#pragma once

#include <string>
#include "ImageTexture2DArray.h"
#include <vector>
#include <memory>
#include "math3d.h"

class Mesh{

    Mesh(const Mesh&) = delete;
    void operator=(const Mesh&) = delete;
    void drawHelper( GLuint vao, unsigned multiplier, GLenum mode, GLenum type);

    Mesh();
    
    class Helpers;
    friend class Helpers;
    
    class Material;
    class MaterialInfo;
    
    std::vector< MaterialInfo > materialList;
    GLuint vao;
    GLuint avao;    //vao with adjacency
    
  public:
  
    std::vector<vec3> lightPositions;
    Mesh(std::string fname);
    ~Mesh();
    void draw(GLenum mode = GL_TRIANGLES, GLenum type = GL_UNSIGNED_INT);
    void drawAdjacency(GLenum mode = GL_TRIANGLES, GLenum type = GL_UNSIGNED_INT);
    static Mesh* fromObjData(const char* data);
};
