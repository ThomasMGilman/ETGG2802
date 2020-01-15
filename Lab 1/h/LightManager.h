
#pragma once

#include "math3d.h"
#include <vector>
#include <memory>
#include "Text.h"

class LightManager{
    std::vector<vec4> positions;
    std::vector<vec4> colors;
    std::vector<vec4> spotlights;
    std::vector<std::shared_ptr<Text>> labels;  //for debugging
public:
    static const int MAX_LIGHTS = 8;        //must match what's #define'd in shader
    vec3 attenuation{1,0,0.0625};
    LightManager();
    void setUniforms();
    void setPosition(unsigned index, vec3 pos, bool positional);
    void setColor( unsigned index, vec3 color);
    vec3 getColor( unsigned index);
    void setSpotlight( unsigned index, vec3 spotDirection, float spotFadeAngle, float spotCutoffAngle);
    //for debugging: Draw a sphere at each light's position
    void draw(int winWidth, int winHeight,const mat4& viewMatrix, const mat4& projMatrix);
};

