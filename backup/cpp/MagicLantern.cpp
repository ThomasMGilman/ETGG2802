#include <stdafx.h>
#include "MagicLantern.h"

MagicLantern::MagicLantern(vec3 pos ) : GameObject(pos)
{
    this->set_animated(true);
    if( MagicLantern::mesh == nullptr )
        MagicLantern::mesh = new Mesh("magiclantern.obj");
}

void MagicLantern::draw(){
    this->draw_setup();
    MagicLantern::mesh->draw();
    stencil_draw();
}

void MagicLantern::stencil_draw()
{
    if (using_stencil_buffer())
    {
        setup_stencil_first_draw();
        MagicLantern::mesh->draw();
        setup_stencil_second_draw();
        MagicLantern::mesh->draw();
        cleanup_stencil_draw();
    }
}

Mesh* MagicLantern::mesh = nullptr;
