
#include "Camera.h"
#include <cmath>

Camera::Camera(vec3 eye, vec3 coi, vec3 up )
{
    this->lookAt(eye,coi,up);
    this->aspectRatio = 1.0f;
    this->hither = 0.1f;
    this->yon = 100.0f;
    this->P = 1.0f + 2.0f * this->yon / (this->hither - this->yon);
    this->Q = 2.0f * this->hither * this->yon / (this->hither - this->yon);
    this->fov_v = radians(30.0f);
    this->fov_h = aspectRatio * this->fov_v;
    this->updateProjMatrix();
}

        
void Camera::lookAt(vec3 eye, vec3 coi, vec3 up)
{
    this->eye = vec4(eye.xyz(),1.0f);
    this->look = vec4( normalize(coi.xyz()-this->eye.xyz()), 0 );
    this->right = vec4( normalize(cross(this->look.xyz(),up.xyz())), 0 );
    this->up = vec4( cross( this->right.xyz(), this->look.xyz() ), 0 );
    this->updateViewMatrix();
}

    
void Camera::updateViewMatrix()
{
    //lhs
    //~ this->viewMatrix = mat4(
        //~ this->right.x, this->up.x, this->look.x, 0,
        //~ this->right.y, this->up.y, this->look.y, 0,
        //~ this->right.z, this->up.z, this->look.z, 0,
        //~ -dot(this->eye,this->right), -dot(this->eye,this->up), -dot(this->eye,this->look), 1.0f);
    //rhs
    this->viewMatrix = mat4(
        this->right.x, this->up.x, -this->look.x, 0,
        this->right.y, this->up.y, -this->look.y, 0,
        this->right.z, this->up.z, -this->look.z, 0,
        -dot(this->eye,this->right), -dot(this->eye,this->up), dot(this->eye,this->look), 1.0f);
}

void Camera::updateProjMatrix()
{
    //lhs
    //~ float P = -( 1.0f + (2.0f*this->yon)/(this->hither-this->yon) );
    //~ float Q = 2.0f * this->hither * this->yon / ( this->hither - this->yon );
    //rhs
    this->projMatrix = mat4(
        1.0f/std::tan(this->fov_h),     0,                          0,       0,
        0,                              1.0f/std::tan(this->fov_v),  0,       0,
        0,                              0,                          P,      -1,
        0,                              0,                          Q,       0
    );
}
    
void Camera::setUniforms()
{
    Program::setUniform("viewMatrix", this->viewMatrix);
    Program::setUniform("projMatrix", this->projMatrix);
    Program::setUniform("eyePos",this->eye.xyz());
    Program::setUniform("P", P);
    Program::setUniform("Q", Q);
}

void Camera::strafe(float dr, float du, float dl)
{
    vec4 delta = dr * this->right + du * this->up + dl * this->look;
    this->eye = this->eye + delta;
    this->updateViewMatrix();
}

void Camera::turn(float amt)
{
    mat4 m = axisRotation( vec3(0,1,0), amt );
    this->right = this->right * m;
    this->up = this->up * m;
    this->look = this->look * m;
    this->updateViewMatrix();
}

void Camera::tilt(float amt){
    mat4 m = axisRotation( this->right,amt );
    this->up = this->up * m;
    this->look = this->look * m;
    this->updateViewMatrix();
}

void Camera::strafeNoUpDown(float dr, float du, float dl )
{
    auto delta = dr * this->right + du * this->up + dl * this->look;
    delta.y = 0;
    this->eye = this->eye + delta;
    this->updateViewMatrix();
}
