#include "glfuncs.h"

#pragma once

class Sampler{
  private:
    Sampler(const Sampler&) = delete;
    void operator=(const Sampler&) = delete;
  public:
    enum class Type { MIPMAP, LINEAR, NEAREST };
    enum class Wrap { CLAMP, REPEAT };
    GLuint samp;
    Sampler(Type type = Type::MIPMAP, Wrap wrap = Wrap::REPEAT);
    ~Sampler();
    void bind(unsigned unit);
};
