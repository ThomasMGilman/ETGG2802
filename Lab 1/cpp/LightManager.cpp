
#include "LightManager.h"
#include "Program.h"
#include "Mesh.h"
#include "icosphere.h"

namespace {
    Mesh* icosphere = nullptr;
};

    
LightManager::LightManager(){
    this->positions.resize(LightManager::MAX_LIGHTS);
    this->colors.resize(LightManager::MAX_LIGHTS);
    this->spotlights.resize(LightManager::MAX_LIGHTS);
    for(unsigned i=0;i<LightManager::MAX_LIGHTS;++i){
        this->colors[i].w=-1.0f;            //cosine spotlight begin fade angle
        this->spotlights[i].z = 1.0f;       //spotlight axis z
        this->spotlights[i].w = -1.0f;      //cosine spotlight end fade angle
        char tmp[2];
        tmp[0] = (char)('0'+(char)i);
        tmp[1] = 0;
        this->labels.push_back( std::make_shared<Text>( 12, vec2(0,0),"Light "+std::string(tmp)));
    }
}

void LightManager::setUniforms(){
    Program::setUniform("lightPositions[0]", this->positions);
    Program::setUniform("lightColors[0]", this->colors);
    Program::setUniform("spotlightDirections[0]", this->spotlights);
    Program::setUniform("attenuation", attenuation );
}

void LightManager::setPosition(unsigned index, vec3 pos, bool positional)
{
    vec4 tmp( pos, positional ? 1.0f : 0.0f );
    this->positions[index] = tmp;
}

void LightManager::setColor( unsigned index, vec3 color) 
{
    this->colors[index].setXYZ( color.xyz() );
}
    

vec3 LightManager::getColor( unsigned index)
{
    return this->colors[index].xyz();
}

//angles are in degrees
void LightManager::setSpotlight( unsigned index, vec3 spotDirection, float spotFadeAngle, float spotCutoffAngle)
{
    this->spotlights[index].setXYZ(spotDirection);
    this->spotlights[index].w = std::cos(radians(spotCutoffAngle));
    this->colors[index].w = std::cos( radians(spotFadeAngle) );
}

void LightManager::draw(int winWidth, int winHeight,const mat4& viewMatrix, const mat4& projMatrix){
    if( !icosphere ){
        icosphere = Mesh::fromObjData(icosphereObjData);
    }
    
    mat4 S = scaling(0.125,0.125,0.125);
    for(unsigned i=0;i<MAX_LIGHTS;++i){
        auto& p = positions[i];
        Program::setUniform("worldMatrix", S * translation( p.xyz() ) );
        //icosphere->draw();
        vec4 pp(p.xyz(),1.0);
        pp = pp * viewMatrix * projMatrix;
        pp /= pp.w;
        vec2 pp2 = pp.xy();
        pp2 += vec2(1.0f,1.0f);
        pp2 *= 0.5f;
        pp2 *= vec2( winWidth, winHeight );
        this->labels[i]->setPosition(pp2);
        this->labels[i]->draw(winWidth,winHeight,pp.z);
    }

}
