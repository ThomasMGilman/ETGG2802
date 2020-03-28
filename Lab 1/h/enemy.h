#pragma once
#include <stdafx.h>
#include <game_object.h>

class Enemy : public GameObject
{
private:
	static Mesh* mesh;
protected:
public:
	Enemy(vec3 pos);

	void draw() override;
};
