#pragma once
#include <stdafx.h>
#include <game_object.h>

class MagicLantern : public GameObject
{
    static Mesh* mesh;
  public:
    MagicLantern(vec3 pos);

    void draw() override;

    void stencil_draw() override;
};

