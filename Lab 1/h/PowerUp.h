#pragma once

#include "Mesh.h"
#include "math3d.h"

class PowerUp
{
	vec3 pos;
	float angle;
	static Mesh* mesh;

	public:
		PowerUp(vec3 pos);
		void update(int elapsed);
		void draw();
};