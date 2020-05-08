#include <stdafx.h>
#include <game_object.h>

GameObject::GameObject(vec3 startingPoint,  vec3 scal, 
	bool moving, vec3 vel, 
	bool dying, float lifetime, 
	bool rotating, float angleOfRotation, float rotAmount,
	bool animated, float startingFrame, float animationSpeed,
	bool useNoise)
{
	this->scal = scal;
	this->angle = angleOfRotation;
	this->rotationAmount = rotAmount;
	this->rotating = rotating;
	this->moving = moving;
	this->pos = startingPoint;
	this->vel = vel;
	this->dying = dying;
	this->lifetime = lifetime;
	this->animated = animated;
	this->frameNum = startingFrame;
	this->animationSpeed = animationSpeed;
	this->usingNoise = useNoise;
}

void GameObject::update_world_mat()
{
	if(orientationChange)
	{
		this->worldMat = scaling(this->scal) * 
			axisRotation(this->rotationAxis, this->angle) * 
			translation(this->pos);
		orientationChange = false;
	}	
}

void GameObject::update(int elapsed)
{
	float elapsedf = (float)elapsed;
	if (this->dying)
		lifetime -= elapsedf;
	if (this->moving)
	{
		this->pos += elapsedf * this->vel;
		orientationChange = true;
	}
	if (this->rotating)
	{
		this->angle += this->rotationAmount * elapsedf;
		if (this->angle >= 360) this->angle -= 360;
		orientationChange = true;
	}
	if (this->animated)
	{
		this->frameNum += this->animationSpeed * elapsedf;
		if (this->frameNum > 60) this->frameNum -= 60;
	}
}

GameObject::~GameObject()
{

}

void GameObject::set_using_stencil_buffer(int forceStencilColorFlag, vec4 forceColor, vec3 stencilScale,
	GLenum firstSFAction, GLenum firstZFAction, GLenum firstZPAction,
	GLenum firstStencilFunc, GLint firstStencilReference, GLuint firstStencilMask,
	GLenum secondSFAction, GLenum secondZFAction, GLenum secondZPAction,
	GLenum secondStencilFunc, GLint secondStencilReference, GLuint secondStencilMask)
{
	usingStencilBuffer = true;
	this->forceStencilColorFlag = forceStencilColorFlag;
	this->stencilColor = forceColor;
	this->stencilScale = stencilScale;
	this->firstStencilFunc = firstStencilFunc;
	this->firstStencilReference = firstStencilReference;
	this->firstStencilMask = firstStencilMask;
	this->firstSFAction = firstSFAction;
	this->firstZFAction = firstZFAction;
	this->firstZPAction = firstZPAction;

	this->secondStencilFunc = secondStencilFunc;
	this->secondStencilReference = secondStencilReference;
	this->secondStencilMask = secondStencilMask;
	this->secondSFAction = secondSFAction;
	this->secondZFAction = secondZFAction;
	this->secondZPAction = secondZPAction;
}

void GameObject::turn(float amount)
{
	this->angle += amount;
	orientationChange = true;
}

void GameObject::set_uniforms()
{
	Program::setUniform("worldMatrix", this->worldMat);
	if (animated) Program::setUniform("sliceNumber", this->frameNum);
}

void GameObject::draw_setup()
{
	this->update_world_mat();
	this->set_uniforms();
}

void GameObject::setup_stencil_first_draw()
{
	glStencilFunc(firstStencilFunc, firstStencilReference, firstStencilMask);
	glStencilOp(firstSFAction, firstZFAction, firstZPAction);
}

void GameObject::setup_stencil_second_draw()
{
	glStencilFunc(secondStencilFunc, secondStencilReference, secondStencilMask);
	glStencilOp(secondSFAction, secondZFAction, secondZPAction);
	Program::setUniform("worldMatrix", scaling(get_stencil_scale()) * this->worldMat);
	Program::setUniform("forceStencilColorFlag", forceStencilColorFlag);
	Program::setUniform("forceStencilColor", stencilColor);
}

void GameObject::cleanup_stencil_draw()
{
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	Program::setUniform("forceStencilColorFlag", 0);
}
