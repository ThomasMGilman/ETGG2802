#include <stdafx.h>
#include <enemy.h>

Enemy::Enemy(vec3 pos) : GameObject(pos)
{
	if (Enemy::mesh == nullptr)
		Enemy::mesh = new Mesh("toothyjaws.glb");
}

void Enemy::draw()
{
	this->draw_setup();
	Enemy::mesh->draw();
	stencil_draw();
}

void Enemy::stencil_draw()
{
	if (using_stencil_buffer())
	{
		setup_stencil_first_draw();
		Enemy::mesh->draw();
		setup_stencil_second_draw();
		Enemy::mesh->draw();
		cleanup_stencil_draw();
	}
}

Mesh* Enemy::mesh = nullptr;