#pragma once
#include <stdafx.h>
#include <PowerUp.h>
#include <Camera.h>

class LightManager{
    std::vector<float> light_yons;
    std::vector<float> light_hithers;
    std::vector<float> light_yonMinusHithers;
    std::vector<vec4> positions;
    std::vector<vec4> rights;
    std::vector<vec4> spotlights;
    std::vector<vec4> colors;
    std::vector<mat4> light_viewMatrices;
    std::vector<mat4> light_projMatrices;
    std::vector<std::shared_ptr<Text>> labels;  //for debugging
    vec3 cois[6] = { vec3(1,0,0), vec3(-1,0,0),  vec3(0,1,0),
                vec3(0,-1,0),  vec3(0,0,1),  vec3(0,0,-1) };
    vec3 ups[6] = { vec3(0,-1,0),  vec3(0,-1,0),  vec3(0,0,1),
               vec3(0,0,-1),  vec3(0,-1,0),  vec3(0,-1,0) };
    Camera lightCam{vec3(0), vec3(0), vec3(0), 45.0f, 1.0f, 200.0f, 0.5f};

public:
    static const int MAX_LIGHTS = 8;        //must match what's #define'd in shader
    vec3 attenuation{1,0,0.0625};

    LightManager();

    void setUniforms();

    void setPosition( unsigned index, vec3 pos, bool positional);

    vec3 getPosition( unsigned index);

    vec3 getDirection( unsigned index);

    void lookAt( unsigned index);

    void setLightYon( unsigned index, float yon);

    void setLightHither( unsigned index, float hither);

    void setLightYonMinusHither( unsigned index, float yonMinusHither);

    void setColor( unsigned index, vec3 color);

    vec3 getColor( unsigned index);

    void setSpotlight( unsigned index, vec3 spotDirection, float spotFadeAngle, float spotCutoffAngle);

    //for debugging: Draw a sphere at each light's position
    void draw( int winWidth, int winHeight,const mat4& viewMatrix, const mat4& projMatrix);

    void print_light_info();
};

