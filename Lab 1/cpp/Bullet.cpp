#include <stdafx.h>
#include "Bullet.h"

Bullet::Bullet(vec3 startingPoint, vec3 vel) : GameObject(startingPoint, vec3(.1), true, vel, true)
{
    if( Bullet::mesh == nullptr )
        Bullet::mesh = new Mesh("bullet.glb");
}

void Bullet::draw(){
    this->draw_setup();
    std::cout << "This matrix: " << this->get_world_mat() << std::endl;
    Bullet::mesh->draw();
}

std::shared_ptr<ImageTexture2DArray> Bullet::get_diffuse_texture()
{
    return Bullet::mesh->get_diffuse_texture();
}

std::shared_ptr<ImageTexture2DArray> Bullet::get_emissive_texture()
{
    return Bullet::mesh->get_emissive_texture();
}

std::shared_ptr<ImageTexture2DArray> Bullet::get_roughness_texture()
{
    return Bullet::mesh->get_roughness_texture();
}

std::shared_ptr<ImageTexture2DArray> Bullet::get_normal_texture()
{
    return Bullet::mesh->get_normal_texture();
}
 
Mesh* Bullet::mesh = nullptr;
