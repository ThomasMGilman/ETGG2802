#pragma once
#include <stdafx.h>

class SkyBox
{
	vec3 pos;
	float angle;
	static Mesh* mesh;

public:
	SkyBox(vec3 pos);
	void update(int elapsed, bool enabled = false);
	void draw();
};