
#pragma once

#include "glfuncs.h"
#include "ImageTexture2DArray.h"
#include "Program.h"

class ParticleSystem{
    static GLuint vao;
    static ImageTexture2DArray* tex;
    static Program* prog;
    float billBoardSize = 100, halfBBX, halfBBY;
    static const int NUM_PARTICLES = 100;
    static const int MAX_LIFE = 1000;
    vec3 origin;
    int totalElapsed;
public:
    ParticleSystem(vec3 origin);
    void update(int elapsed);
    bool isDead();
    void draw();
};

            
