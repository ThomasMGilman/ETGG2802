#pragma once
#include <stdafx.h>
#include <MagicLantern.h>

class LightManager{
    std::vector<float> light_yons;
    std::vector<float> light_hithers;
    std::vector<float> light_yonMinusHithers;
    std::vector<vec4> positions;
    std::vector<vec4> looks;
    std::vector<vec4> rights;
    std::vector<vec4> ups;
    std::vector<vec4> spotlights;
    std::vector<vec4> colors;
    std::vector<mat4> light_viewMatrices;
    std::vector<mat4> light_projMatrices;
    std::vector<std::shared_ptr<Text>> labels;  //for debugging
    MagicLantern torch{ vec3(0,0,0) };
public:
    static const int MAX_LIGHTS = 8;        //must match what's #define'd in shader
    vec3 attenuation{1,0,0.0625};

    LightManager();

    void setUniforms();

    void setPosition( unsigned index, vec3 pos, bool positional);

    vec3 getPosition( unsigned index);

    vec3 getDirection( unsigned index);

    void strafe( unsigned index, float dr, float du, float dl);

    void strafeNoUpDown( unsigned index, float dr, float du, float dl);

    void turn( unsigned index, float amt);

    void lookAt( unsigned index, vec3 pos, vec3 coi, vec3 up);

    void updateViewMatrix( unsigned index);

    void setLightYon( unsigned index, float yon);

    void setLightHither( unsigned index, float hither);

    void setLightYonMinusHither( unsigned index, float yonMinusHither);

    void setViewMatrix( unsigned index, mat4 viewMat);

    void setProjMatrix( unsigned index, mat4 projMat);

    void setColor( unsigned index, vec3 color);

    vec3 getColor( unsigned index);

    void setSpotlight( unsigned index, vec3 spotDirection, float spotFadeAngle, float spotCutoffAngle);

    void debug_mesh_draw( unsigned index);

    //for debugging: Draw a sphere at each light's position
    void draw( int winWidth, int winHeight,const mat4& viewMatrix, const mat4& projMatrix);

    void print_light_info();
};

