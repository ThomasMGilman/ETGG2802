#include <stdafx.h>
#include <game_state.h>
#include <torch.h>

Torch::Torch(vec3 pos, bool useNoise) : GameObject(pos)
{
    set_using_noise(useNoise);
    if (Torch::mesh == nullptr)
    {
        Torch::mesh = new Mesh("torch.glb");
        Torch::prog = new Program("main_vs.txt", "fire_fs.txt");
    }  
}

Torch::~Torch()
{
}

void Torch::draw()
{
    auto oldprog = Program::current;
    Torch::prog->use();
    draw_setup();

    GAME_STATE->set_uniform<int>("doNoise", using_noise());
    Torch::mesh->draw();
    GAME_STATE->set_uniform<int>("doNoise", 0);

    if (oldprog)
        oldprog->use();
}

void Torch::stencil_draw()
{
}

std::shared_ptr<ImageTexture2DArray> Torch::get_diffuse_texture()
{
    return Torch::mesh->get_diffuse_texture();
}

std::shared_ptr<ImageTexture2DArray> Torch::get_emissive_texture()
{
    return Torch::mesh->get_emissive_texture();
}

std::shared_ptr<ImageTexture2DArray> Torch::get_roughness_texture()
{
    return Torch::mesh->get_roughness_texture();
}

std::shared_ptr<ImageTexture2DArray> Torch::get_normal_texture()
{
    return Torch::mesh->get_normal_texture();
}

Mesh* Torch::mesh = nullptr;
Program* Torch::prog = nullptr;