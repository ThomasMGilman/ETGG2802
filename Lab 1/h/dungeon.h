#pragma once
#include <stdafx.h>
#include <game_object.h>

class Dungeon : public GameObject
{
private:
	static Mesh* mesh;
protected:
public:
	Dungeon(vec3 pos);

	int get_num_light_positions() { return mesh->lightPositions.size(); }

	vec3 get_light_position(int index) { if(index < get_num_light_positions()) return mesh->lightPositions[index]; }

	void draw() override;
};