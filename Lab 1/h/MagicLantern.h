
#pragma once

#include "Mesh.h"
#include "math3d.h"

class MagicLantern 
{
    vec3 pos;
    float frameNum;
    static Mesh* mesh;
    
  public:
    
    MagicLantern(vec3 pos);
    void update(int elapsed);
    void draw();
};

