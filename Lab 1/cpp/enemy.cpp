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
}

Mesh* Enemy::mesh = nullptr;