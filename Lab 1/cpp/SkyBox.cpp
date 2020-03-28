#include <stdafx.h>
#include "SkyBox.h"

SkyBox::SkyBox(vec3 pos)
{
	this->pos = pos;
	this->angle = 0;
	if (SkyBox::mesh == nullptr)
		SkyBox::mesh = new Mesh("cube.obj");
}

void SkyBox::update(int elapsed, bool enabled)
{
	if (enabled)
		this->angle += 0.001f * float(elapsed);
	if (this->angle >= 360) this->angle -= 360;
}

void SkyBox::draw()
{
	Program::setUniform("worldMatrix", axisRotation(vec4(0, 1, 0, 0), this->angle) * translation(this->pos));
	SkyBox::mesh->draw();
}

Mesh* SkyBox::mesh = nullptr;