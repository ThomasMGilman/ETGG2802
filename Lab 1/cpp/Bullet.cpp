
#include "Bullet.h"
#include "Program.h"

Bullet::Bullet(vec3 startingPoint, vec3 vel)
{
    halfBBX = billBoardSize / 2;
    halfBBY = halfBBY;
    this->lifetime = 750;
    this->pos = startingPoint;
    this->vel = vel;
    if( mesh == nullptr )
        mesh = new Mesh("bullet.glb");
}

void Bullet::update(int elapsed){
    this->lifetime -= elapsed;
    this->pos = this->pos + float(elapsed) * 0.005f * this->vel;
}

void Bullet::draw(){
    Program::setUniform("worldMatrix", scaling(vec3(0.1f,0.1f,0.1f)) * translation( this->pos ) );
    Program::setUniform("halfBBX", halfBBX);
    Program::setUniform("halfBBY", halfBBY);
    Bullet::mesh->draw(GL_POINTS);
}

bool Bullet::isDead(){
    return this->lifetime <= 0;
}
 
Mesh* Bullet::mesh = nullptr;
