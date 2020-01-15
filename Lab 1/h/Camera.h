#pragma once

#include "math3d.h"
#include "Program.h"

class Camera{
  public:
  
    vec4 eye;
    vec4 look;
    vec4 right;
    vec4 up;
    
    float aspectRatio;
    float hither;
    float yon;
    float fov_v;
    float fov_h;
    mat4 viewMatrix;
    mat4 projMatrix;
    
    Camera(vec3 eye, vec3 coi, vec3 up );
    void lookAt(vec3 eye, vec3 coi, vec3 up);
    void updateViewMatrix();
    void updateProjMatrix();
    void setUniforms();
    void strafe(float dr, float du, float dl);
    void strafeNoUpDown(float dr, float du, float dl);
    void turn(float amt);
    void tilt(float amt);
};

