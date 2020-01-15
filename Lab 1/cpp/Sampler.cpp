
#include "Sampler.h"
#include <cassert>

Sampler::Sampler(Sampler::Type type, Sampler::Wrap wrap){
    GLuint tmp[1];
    glGenSamplers(1,tmp);
    this->samp = tmp[0];
    
    if( wrap == Sampler::Wrap::REPEAT ){
        glSamplerParameteri( this->samp,
            GL_TEXTURE_WRAP_S, GL_REPEAT );
        glSamplerParameteri( this->samp,
            GL_TEXTURE_WRAP_T, GL_REPEAT );
    } else {
        glSamplerParameteri( this->samp,
            GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glSamplerParameteri( this->samp,
            GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }
    
    if( type == Sampler::Type::MIPMAP ){
        glSamplerParameteri( this->samp, 
            GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri( this->samp, 
            GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else if( type == Sampler::Type::LINEAR ) {
        glSamplerParameteri( this->samp, 
            GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri( this->samp, 
            GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else if( type == Sampler::Type::NEAREST ) {
        glSamplerParameteri( this->samp, 
            GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri( this->samp, 
            GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        assert(0);
    }
}

void Sampler::bind(unsigned unit){
    glBindSampler(unit, this->samp );
}

Sampler::~Sampler(){
    glDeleteSamplers(1,&samp);
}
