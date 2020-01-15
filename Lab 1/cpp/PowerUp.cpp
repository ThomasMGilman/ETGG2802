#include "PowerUp.h"
#include "Program.h"

PowerUp::PowerUp(vec3 pos)
{
	this->pos = pos;
	this->angle = 0;
	if (PowerUp::mesh == nullptr)
		PowerUp::mesh = new Mesh("candycane.glb");
}

void PowerUp::update(int elapsed)
{
	this->angle += 0.001f * float(elapsed);
	if (this->angle >= 360) this->angle -= 360;
}

void PowerUp::draw()
{
	Program::setUniform("worldMatrix", scaling(vec3(.05f, .05f, .05f)) * axisRotation(vec4(0, 1, 0, 0), this->angle) * translation(this->pos));
	PowerUp::mesh->draw();
}

Mesh* PowerUp::mesh = nullptr;