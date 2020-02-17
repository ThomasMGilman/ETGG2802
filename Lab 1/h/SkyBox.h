#pragma once

#include "Mesh.h"
#include "math3d.h"

class SkyBox
{
	vec3 pos;
	float angle;
	static Mesh* mesh;

public:
	SkyBox(vec3 pos);
	void update(int elapsed);
	void draw();
};