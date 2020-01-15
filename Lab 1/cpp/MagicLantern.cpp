
#include "MagicLantern.h"
#include "math3d.h"
#include "Program.h"

MagicLantern::MagicLantern(vec3 pos ){
    this->pos=pos;
    this->frameNum = 0;
    if( MagicLantern::mesh == nullptr )
        MagicLantern::mesh = new Mesh("magiclantern.obj");
}
    
void MagicLantern::update(int elapsed)
{
    this->frameNum += 0.01f*float(elapsed);
    while(this->frameNum > 60)
        this->frameNum -= 60;
}

void MagicLantern::draw(){
    Program::setUniform("worldMatrix", translation( this->pos ));
    Program::setUniform("sliceNumber", this->frameNum);
    MagicLantern::mesh->draw();
}

Mesh* MagicLantern::mesh = nullptr;
