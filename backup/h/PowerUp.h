#pragma once
#include <stdafx.h>
#include <game_object.h>

class PowerUp : public GameObject
{
	static Mesh* mesh;
	public:
		PowerUp(vec3 pos);

		void draw() override;

		void stencil_draw() override;
};