#pragma once
#include <stdafx.h>

class GameObject
{
private:
	int lifetime;
	float angle = 0, rotationAmount = 0, frameNum = 0, animationSpeed = 0.01f;
	bool rotating, moving, dying, animated = false, orientationChange = true, hasMesh = false;
	vec3 pos, vel, scal;
	vec4 rotationAxis = vec4(0,0,0,0);

	mat4 worldMat = mat4::identity();
	
protected:
public:
	GameObject(vec3 startingPoint = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), 
		bool moving = false, vec3 vel = vec3(0, 0, 0),
		bool dying = false, float lifetime = 750, 
		bool rotating = false, float angleOfRotation = 0, float rotAmount = 0.001f,
		bool animated = false, float startingFrame = 0, float animationSpeed = 0.01f );

	~GameObject();

	void update_world_mat();

	virtual void update(int elapsed);

	bool is_moving() { return this->moving; };

	bool is_dying() { return this->dying; };

	bool is_rotating() { return this->rotating; };

	bool is_animated() { return this->animated; };

	bool has_mesh() { return this->hasMesh; };

	virtual bool is_dead() { if (dying) return lifetime < 0; return false; };

	vec3 get_pos() { return this->pos; };

	vec3 set_pos(vec3 newP) { this->pos = newP; orientationChange = true; };

	vec3 get_scale() { return this->scal; };

	vec3 set_scale(vec3 newS) { this->scal = newS; orientationChange = true; };

	vec3 get_velocity() { return this->vel; };

	vec3 set_velocity(vec3 newV) { this->vel = newV; orientationChange = true; };

	mat4 get_world_mat() { return this->worldMat; }

	mat4 set_world_mat(mat4 newWM4) { this->worldMat = newWM4; }

	void set_moving(bool state) { this->moving = state; };

	void set_dying(bool state) { this->dying = state; };

	void set_rotating(bool state) { this->rotating = state; };

	void set_animated(bool state) { this->animated = state; };

	void update_rotation_axis(vec4 r) { rotationAxis = r; };

	void update_rotation_axis(float x, float y, float z, float w) { rotationAxis = vec4(x, y, z, w); };

	virtual void set_uniforms();
	
	void draw_setup();

	virtual void draw() = NULL;

	virtual std::shared_ptr<ImageTexture2DArray> get_diffuse_texture() { return nullptr; };

	virtual std::shared_ptr<ImageTexture2DArray> get_emissive_texture() { return nullptr; };

	virtual std::shared_ptr<ImageTexture2DArray> get_roughness_texture() { return nullptr; };

	virtual std::shared_ptr<ImageTexture2DArray> get_normal_texture() { return nullptr; };
};