#pragma once
#include <stdafx.h>

class ParticleSystem{
    static GLuint vao;
    static ImageTexture2DArray* tex;
    static Program* prog;
    static const int NUM_PARTICLES = 100;
    static const int MAX_LIFE = 1000;
    vec2 size, halfSize;
    vec3 origin;
    int totalElapsed;
public:
    ParticleSystem(vec3 origin);
    void update(int elapsed);
    bool is_dead();
    void draw();
};

            
