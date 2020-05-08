#pragma once
#include <stdafx.h>
#include <game_object.h>

class Bullet : public GameObject
{
    static Mesh* mesh;
  public:
    float billBoardSize = .5;

    Bullet(vec3 startingPoint, vec3 vel);

    void draw() override;

    void stencil_draw() override;

    std::shared_ptr<ImageTexture2DArray> get_diffuse_texture() override;

    std::shared_ptr<ImageTexture2DArray> get_emissive_texture() override;

    std::shared_ptr<ImageTexture2DArray> get_roughness_texture() override;

    std::shared_ptr<ImageTexture2DArray> get_normal_texture() override;
};
