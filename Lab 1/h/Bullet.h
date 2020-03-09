
#pragma once

#include "math3d.h"
#include "Mesh.h"

class Bullet {
    static Mesh* mesh;
  public:
    float billBoardSize = .5, halfBBX, halfBBY;
    int lifetime;
    vec3 pos;
    vec3 vel;
    
    Bullet(vec3 startingPoint, vec3 vel);
    void update(int elapsed);
    void draw();
    bool isDead();
};
