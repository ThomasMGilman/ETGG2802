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
}

Mesh* MagicLantern::mesh = nullptr;
