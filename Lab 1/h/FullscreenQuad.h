#pragma once

#include "glfuncs.h"

class Globals;

class FullscreenQuad{
    friend Globals;
    FullscreenQuad(const FullscreenQuad&) = delete;
    void operator=(const FullscreenQuad&) = delete;
  public:
    GLuint vao;
    FullscreenQuad();
    ~FullscreenQuad();
    void draw();
};
