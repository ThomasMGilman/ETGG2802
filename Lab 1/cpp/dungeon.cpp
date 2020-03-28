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
}

Mesh* Dungeon::mesh = nullptr;