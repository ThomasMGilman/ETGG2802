#include <stdafx.h>
#include "PowerUp.h"

PowerUp::PowerUp(vec3 pos) : GameObject(pos, vec3(.05))
{
	this->update_rotation_axis(vec4(0, 1, 0, 0));
	this->set_rotating(true);
	if (PowerUp::mesh == nullptr)
		PowerUp::mesh = new Mesh("candycane.glb");
}

void PowerUp::draw()
{
	draw_setup();
	PowerUp::mesh->draw();
	stencil_draw();
}

void PowerUp::stencil_draw()
{
	if (using_stencil_buffer())
	{
		setup_stencil_first_draw();
		PowerUp::mesh->draw();
		setup_stencil_second_draw();
		PowerUp::mesh->draw();
		cleanup_stencil_draw();
	}
}

Mesh* PowerUp::mesh = nullptr;