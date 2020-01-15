#pragma once

#include "glfuncs.h"

class FullscreenQuad{
    FullscreenQuad(const FullscreenQuad&) = delete;
    void operator=(const FullscreenQuad&) = delete;
  public:
    GLuint vao;
    FullscreenQuad();
    ~FullscreenQuad();
    void draw();
};
