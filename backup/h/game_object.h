#pragma once
#include <stdafx.h>

class GameObject
{
private:
	int lifetime;
	float angle = 0, rotationAmount = 0, frameNum = 0, animationSpeed = 0.01f;
	bool rotating, moving, dying, animated, usingNoise, usingStencilBuffer = false, orientationChange = true, hasMesh = false;
	vec3 pos, vel, scal;
	vec4 rotationAxis = vec4(0,0,0,0);

	// Stencil Variables
	int forceStencilColorFlag = 0;
	vec3 stencilScale = vec3(1, 1, 1);
	vec4 stencilColor = vec4(0, 0, 0, 0);
	GLenum secondStencilFunc, secondSFAction, secondZFAction, secondZPAction;
	GLenum firstStencilFunc, firstSFAction, firstZFAction, firstZPAction;
	GLint firstStencilReference, secondStencilReference;
	GLuint firstStencilMask, secondStencilMask;

	mat4 worldMat = mat4::identity();
	
protected:
public:
	GameObject(vec3 startingPoint = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), 
		bool moving = false, vec3 vel = vec3(0, 0, 0),
		bool dying = false, float lifetime = 750, 
		bool rotating = false, float angleOfRotation = 0, float rotAmount = 0.001f,
		bool animated = false, float startingFrame = 0, float animationSpeed = 0.01f,
		bool useNoise = false);

	~GameObject();

	void update_world_mat();

	virtual void update(int elapsed);

	bool is_moving() { return this->moving; };

	bool is_dying() { return this->dying; };

	bool is_rotating() { return this->rotating; };

	bool is_animated() { return this->animated; };

	bool using_noise() { return this->usingNoise; };

	bool has_mesh() { return this->hasMesh; };

	bool using_stencil_buffer() { return this->usingStencilBuffer; };

	virtual bool is_dead() { if (dying) return lifetime < 0; return false; };

	vec3 get_pos() { return this->pos; };

	void set_pos(vec3 newP) { this->pos = newP; orientationChange = true; };

	vec3 get_scale() { return this->scal; };

	void set_scale(vec3 newS) { this->scal = newS; orientationChange = true; };

	vec3 get_velocity() { return this->vel; };

	void set_velocity(vec3 newV) { this->vel = newV; orientationChange = true; };

	vec3 get_stencil_scale() { return this->stencilScale; };

	void set_stencil_scale(vec3 newS) { this->stencilScale = newS; };

	mat4 get_world_mat() { return this->worldMat; }

	void set_world_mat(mat4 newWM4) { this->worldMat = newWM4; }

	void set_moving(bool state) { this->moving = state; };

	void set_dying(bool state) { this->dying = state; };

	void set_rotating(bool state) { this->rotating = state; };

	void set_animated(bool state) { this->animated = state; };

	void set_using_noise(bool state) { this->usingNoise = state; };

	/*
	Actions:
	----GL_KEEP: Leave stencil buffer alone
	----GL_ZERO: Set stencil buffer to zero
	----GL_INCR: Increment stencil buffer value; if it overflows, clamp to max value
	----GL_DECR: Decrement stencil buffer value; if it underflows, clamp to 0
	----GL_INVERT: Flip stencil buffer bits (0 leftrightarrow 1)
	----GL_INCR_WRAP: Increment; if it overflows, wrap to zero
	----GL_DECR_WRAP: Decrement; if it underflows, wrap to max value
	----GL_REPLACE: Set stencil buffer to reference value

	GL_NEVER: Always fails (returns false)
	GL_ALWAYS: Always succeeds (returns true)
	GL_EQUAL: Succeeds if reference value equals value in stencil buffer at current pixel location
	GL_NOTEQUAL: Succeeds if reference != value in stencil buffer
	GL_GREATER: Succeeds if reference > value in stencil buffer
	GL_LESS: Succeeds if reference < value in buffer
	GL_GEQUAL: Succeeds if reference >= value in buffer
	GL_LEQUAL: Succeeds if reference <= value in buffer
	*/
	void set_using_stencil_buffer(int forceStencilColorFlag, vec4 forceColor, vec3 stencilScale = vec3(1.05),
		GLenum firstSFAction = GL_KEEP, GLenum firstZFAction = GL_KEEP, GLenum firstZPAction = GL_REPLACE,
		GLenum firstStencilFunc = GL_ALWAYS, GLint firstStencilReference = 1, GLuint firstStencilMask = 0xff,
		GLenum secondSFAction = GL_KEEP, GLenum secondZFAction = GL_KEEP, GLenum secondZPAction = GL_INCR,
		GLenum secondStencilFunc = GL_EQUAL, GLint secondStencilReference = 0, GLuint secondStencilMask = 0xff);

	void turn(float amount);

	void update_rotation_axis(vec4 r) { rotationAxis = r; };

	void update_rotation_axis(float x, float y, float z, float w) { rotationAxis = vec4(x, y, z, w); };

	virtual void set_uniforms();
	
	void draw_setup();

	void setup_stencil_first_draw();
	
	void setup_stencil_second_draw();

	void cleanup_stencil_draw();

	virtual void stencil_draw() = 0;

	virtual void draw() = 0;

	virtual std::shared_ptr<ImageTexture2DArray> get_diffuse_texture() { return nullptr; };

	virtual std::shared_ptr<ImageTexture2DArray> get_emissive_texture() { return nullptr; };

	virtual std::shared_ptr<ImageTexture2DArray> get_roughness_texture() { return nullptr; };

	virtual std::shared_ptr<ImageTexture2DArray> get_normal_texture() { return nullptr; };
};