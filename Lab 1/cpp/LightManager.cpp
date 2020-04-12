#include <stdafx.h>
#include "LightManager.h"

namespace {
    Mesh* icosphere = nullptr;
};

    
LightManager::LightManager(){
    this->positions.resize(LightManager::MAX_LIGHTS);
    this->rights.resize(LightManager::MAX_LIGHTS);
    this->ups.resize(LightManager::MAX_LIGHTS);
    this->looks.resize(LightManager::MAX_LIGHTS);
    this->spotlights.resize(LightManager::MAX_LIGHTS);
    this->colors.resize(LightManager::MAX_LIGHTS);
    this->light_viewMatrices.resize(LightManager::MAX_LIGHTS);
    this->light_projMatrices.resize(LightManager::MAX_LIGHTS);
    this->light_hithers.resize(LightManager::MAX_LIGHTS);
    this->light_yons.resize(LightManager::MAX_LIGHTS);
    this->light_yonMinusHithers.resize(LightManager::MAX_LIGHTS);

    for(unsigned i=0;i<LightManager::MAX_LIGHTS;++i){
        this->colors[i].w=-1.0f;            //cosine spotlight begin fade angle
        this->spotlights[i].z = 1.0f;       //spotlight axis z
        this->spotlights[i].w = -1.0f;      //cosine spotlight end fade angle
        lookAt(i, vec3(0, 0, 0), vec3(0,0,1), vec3(0, 1, 0));
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
    updateViewMatrix(index);
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

void LightManager::strafe(unsigned index, float dr, float du, float dl)
{
    vec4 delta = dr * this->rights[index] + du * this->ups[index] + dl * this->looks[index];
    this->positions[index] = this->positions[index] + delta;
    this->updateViewMatrix(index);
}

void LightManager::strafeNoUpDown(unsigned index, float dr, float du, float dl)
{
    auto delta = dr * this->rights[index] + du * this->ups[index] + dl * this->looks[index];
    delta.y = 0;
    this->positions[index] = this->positions[index] + delta;
    this->updateViewMatrix(index);
}

void LightManager::turn(unsigned index, float amt)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    mat4 m = axisRotation(vec3(0, 1, 0), amt);
    this->rights[index] = this->rights[index] * m;
    this->ups[index] = this->ups[index] * m;
    this->looks[index] = this->looks[index] * m;
    this->updateViewMatrix(index);
}

void LightManager::lookAt(unsigned index, vec3 pos, vec3 coi, vec3 up)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->positions[index] = vec4(pos.xyz(), this->positions[index].w);
    this->looks[index] = vec4(normalize(coi.xyz() - pos.xyz()), 0.0f);
    this->rights[index] = vec4(normalize(cross(this->looks[index].xyz(), up.xyz())), 0.0f);
    this->ups[index] = vec4(cross(this->rights[index].xyz(), this->looks[index].xyz()), 0.0f);
    this->spotlights[index].setXYZ(this->looks[index].xyz());
    this->updateViewMatrix(index);
}

void LightManager::updateViewMatrix(unsigned index)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    //lhs
    //~ this->light_viewMatrices[index] = mat4(
        //~ this->rights[index].x, this->ups[index].x, this->looks[index].x, 0,
        //~ this->rights[index].y, this->ups[index].y, this->looks[index].y, 0,
        //~ this->rights[index].z, this->ups[index].z, this->looks[index].z, 0,
        //~ -dot(this->positions[index],this->rights[index]), -dot(this->positions[index],this->ups[index]), -dot(this->positions[index],this->looks[index]), 1.0f);
    //rhs
    this->light_viewMatrices[index] = mat4(
        this->rights[index].x, this->ups[index].x, -this->looks[index].x, 0.0f,
        this->rights[index].y, this->ups[index].y, -this->looks[index].y, 0.0f,
        this->rights[index].z, this->ups[index].z, -this->looks[index].z, 0.0f,
        -dot(this->positions[index], this->rights[index]), -dot(this->positions[index], this->ups[index]), dot(this->positions[index], this->looks[index]), 1.0f);
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

void LightManager::setViewMatrix( unsigned index, mat4 viewMat)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->light_viewMatrices[index] = viewMat;
}

void LightManager::setProjMatrix(unsigned index, mat4 projMat)
{
    if (index >= LightManager::MAX_LIGHTS)
        throw new std::exception("Trying to access index outside range allocated!!!");
    this->light_projMatrices[index] = projMat;
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
    this->lookAt(index, this->positions[index].xyz(), spotDirection, vec3(0, 1, 0));
}

void LightManager::debug_mesh_draw(unsigned index)
{
    this->torch.set_pos(this->positions[index].xyz());
    this->torch.draw();
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
        std::cout << "\t\tCOI: " << this->looks[i] << std::endl;
        std::cout << "\t\tyon: " << this->light_yons[i] << std::endl;
        std::cout << "\t\thither: " << this->light_hithers[i] << "\n" << std::endl;
    }
    std::cout << "/////////////////////////////////////////////////////////////////////" << std::endl;
}
