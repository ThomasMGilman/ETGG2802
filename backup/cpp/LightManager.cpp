#include <stdafx.h>
#include "LightManager.h"

namespace {
    Mesh* icosphere = nullptr;
};

    
LightManager::LightManager(){
    this->positions.resize(LightManager::MAX_LIGHTS);
    this->rights.resize(LightManager::MAX_LIGHTS);
    this->spotlights.resize(LightManager::MAX_LIGHTS);
    this->colors.resize(LightManager::MAX_LIGHTS);
    this->light_hithers.resize(LightManager::MAX_LIGHTS);
    this->light_yons.resize(LightManager::MAX_LIGHTS);
    this->light_yonMinusHithers.resize(LightManager::MAX_LIGHTS);
    this->light_viewMatrices.resize(LightManager::MAX_LIGHTS * 6);
    this->light_projMatrices.resize(LightManager::MAX_LIGHTS * 6);

    for(unsigned i=0;i<LightManager::MAX_LIGHTS;++i){
        this->colors[i].w=-1.0f;            //cosine spotlight begin fade angle
        this->spotlights[i].z = 1.0f;       //spotlight axis z
        this->spotlights[i].w = -1.0f;      //cosine spotlight end fade angle
        this->light_yons[i] = 200.0f;
        this->light_hithers[i] = 0.01f;
        this->light_yonMinusHithers[i] = this->light_yons[i] - this->light_hithers[i];
        char tmp[2];
        tmp[0] = (char)('0'+(char)i);
        tmp[1] = 0;
        this->labels.push_back( std::make_shared<Text>( 12, vec2(0,0),"Light "+std::string(tmp)));
    }
}

void LightManager::setUniforms(){
    Program::setUniform("light_yons[0]", this->light_yons);
    Program::setUniform("light_hithers[0]", this->light_hithers);
    Program::setUniform("light_yonMinusHithers[0]", this->light_yonMinusHithers);
    Program::setUniform("lightPositions[0]", this->positions);
    Program::setUniform("lightColors[0]", this->colors);
    Program::setUniform("spotlightDirections[0]", this->spotlights);
    Program::setUniform("light_viewMatrices[0]", this->light_viewMatrices);
    Program::setUniform("light_projMatrices[0]", this->light_projMatrices);
    Program::setUniform("attenuation", attenuation );
}

void LightManager::setPosition(unsigned index, vec3 pos, bool positional)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    vec4 tmp( pos, positional ? 1.0f : 0.0f );
    this->positions[index] = tmp;
    this->lookAt(index);
}

vec3 LightManager::getPosition(unsigned index)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    return this->positions[index].xyz();
}

vec3 LightManager::getDirection(unsigned index)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    return this->spotlights[index].xyz();
}

void LightManager::lookAt(unsigned index)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    for (int i = 0; i < 6; i++)
    {
        lightCam.lookAt(this->positions[index].xyz(), this->cois[i].xyz(), this->ups[i].xyz());

        this->light_viewMatrices[i + (index*6)] = lightCam.viewMatrix;
        this->light_projMatrices[i + (index*6)] = lightCam.projMatrix;
    }
}

void LightManager::setLightYon(unsigned index, float yon)
{
    if(index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->light_yons[index] = yon;
}

void LightManager::setLightHither(unsigned index, float hither)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->light_hithers[index] = hither;
}

void LightManager::setLightYonMinusHither(unsigned index, float yonMinusHither)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->light_yonMinusHithers[index] = yonMinusHither;
}

void LightManager::setColor( unsigned index, vec3 color) 
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->colors[index].setXYZ( color.xyz() );
}
    
vec3 LightManager::getColor( unsigned index)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    return this->colors[index].xyz();
}

//angles are in degrees
void LightManager::setSpotlight( unsigned index, vec3 spotDirection, float spotFadeAngle, float spotCutoffAngle)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
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

void LightManager::print_light_info()
{
    std::cout << "/////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "LightManager Info:" << std::endl;
    for (int i = 0; i < LightManager::MAX_LIGHTS; i++)
    {
        std::cout << "\tLight: " << std::to_string(i) << std::endl;
        std::cout << "\t\tPosition: " << this->positions[i] << std::endl;
        std::cout << "\t\tColor: " << this->colors[i] << std::endl;
        std::cout << "\t\tCOI: " << this->cois[i] << std::endl;
        std::cout << "\t\tyon: " << this->light_yons[i] << std::endl;
        std::cout << "\t\thither: " << this->light_hithers[i] << "\n" << std::endl;
    }
    std::cout << "/////////////////////////////////////////////////////////////////////" << std::endl;
}
