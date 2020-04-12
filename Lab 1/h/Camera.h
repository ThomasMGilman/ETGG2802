#pragma once
#include <stdafx.h>

class Camera{
  public:
  
    vec4 eye;
    vec4 look;
    vec4 right;
    vec4 up;
    
    float aspectRatio;
    float hither;
    float yon;
    float yonMinusHither;
    float P;
    float Q;
    float fov_v;
    float fov_h;
    float speedMultiplier = 2;
    mat4 viewMatrix;
    mat4 projMatrix;
    
    Camera(vec3 eye, vec3 coi, vec3 up, float fovAngleD = 30.0f);
    void lookAt(vec3 eye, vec3 coi, vec3 up);
    void updateViewMatrix();
    void updateProjMatrix();
    void setUniforms(std::string prefix = "");
    void strafe(float dr, float du, float dl);
    void strafeNoUpDown(float dr, float du, float dl);
    void turn(float amt);
    void tilt(float amt);
};

