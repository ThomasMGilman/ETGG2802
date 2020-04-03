#include <stdafx.h>
#include <dungeon.h>

Dungeon::Dungeon(vec3 pos) : GameObject(pos)
{
	if (Dungeon::mesh == nullptr)
		Dungeon::mesh = new Mesh("dungeon.glb");
}

void Dungeon::draw()
{
	this->draw_setup();
	Dungeon::mesh->draw();
	stencil_draw();
}

void Dungeon::stencil_draw()
{
	if (using_stencil_buffer())
	{
		setup_stencil_first_draw();
		Dungeon::mesh->draw();
		setup_stencil_second_draw();
		Dungeon::mesh->draw();
		cleanup_stencil_draw();
	}
}

Mesh* Dungeon::mesh = nullptr;