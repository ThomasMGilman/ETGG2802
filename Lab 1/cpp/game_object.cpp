#include <stdafx.h>
#include <game_object.h>

GameObject::GameObject(vec3 startingPoint,  vec3 scal, 
	bool moving, vec3 vel, 
	bool dying, float lifetime, 
	bool rotating, float angleOfRotation, float rotAmount,
	bool animated, float startingFrame, float animationSpeed)
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
