#pragma once
#include <stdafx.h>
#include <game_object.h>

class Torch : public GameObject
{
private:
    static Mesh* mesh;
    static Program* prog;
protected:
public:
	Torch(vec3 pos, bool useNoise);

	~Torch();

    void draw() override;

    void stencil_draw() override;

    std::shared_ptr<ImageTexture2DArray> get_diffuse_texture() override;

    std::shared_ptr<ImageTexture2DArray> get_emissive_texture() override;

    std::shared_ptr<ImageTexture2DArray> get_roughness_texture() override;

    std::shared_ptr<ImageTexture2DArray> get_normal_texture() override;
};