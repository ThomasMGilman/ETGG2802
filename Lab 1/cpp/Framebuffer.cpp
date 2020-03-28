#include <stdafx.h>
#include "Framebuffer.h"
#include "Program.h"
#include <map>
#include <vector>
#include "FullscreenQuad.h"
#include "Image.h"
#include "DataTexture2DArray.h"

namespace{
    
    const char* blurvs = 
"#version 430\n"
"layout(location=0) in vec3 position;\n"
"layout(location=1) in vec2 texCoord;\n"
"out vec2 v_texCoord;\n"
"void main(){\n"
"    gl_Position = vec4(position.xy,-1.0,1.0);\n"
"    v_texCoord = texCoord;\n"
"}\n";

    const char* blurfs = 
"#version 430\n"
"layout(binding=0) uniform sampler2DArray tex;\n"
"in vec2 v_texCoord;\n"
"out vec4 color;\n"
"#define MAX_BLUR_RADIUS 30\n"
"uniform float blurWeights[MAX_BLUR_RADIUS];\n"
"uniform int blurRadius;\n"
"uniform vec2 blurDeltas;\n"
"uniform float blurMultiplier;\n"
"uniform float blurSourceSlice;\n"
"void main(){\n"
"    color = vec4(0);\n"
"    color += blurWeights[0] * texture( tex, vec3( v_texCoord , blurSourceSlice ) );\n"
"    for(int i=1;i<=blurRadius;++i){\n"
"        color += blurWeights[i] * texture( tex, vec3( v_texCoord - i * blurDeltas , blurSourceSlice ) );\n"
"        color += blurWeights[i] * texture( tex, vec3( v_texCoord + i * blurDeltas , blurSourceSlice ) );\n"
"    }\n"
"    color.rgb *= blurMultiplier;\n"
"    color.a=1.0;\n"
"}\n";

    //saved viewport
    int saved_viewport[4];
    //key=(w,h,format), value = Framebuffer
    std::map<std::array<unsigned,3>, Framebuffer*  > blurHelpers;
    //key = kernel size (i.e., radius); value=weights; array for shader
    std::map<int, std::vector<float> > blurWeights;
    const int MAX_BLUR_RADIUS = 30;
    FullscreenQuad* fsquad;
    Program* blurProgram;
    
    
    //compute blur weights for Gaussian blur
    //Returned vector has entries for 0...radius, inclusive
    const std::vector<float>& computeBlurWeights(int radius){
        if( blurWeights.find(radius) == blurWeights.end() ){
            std::vector<float> F(MAX_BLUR_RADIUS);
            float sigma = float(radius) / 3.0f;
            float sum = 0.0f;
            for (int i = 0; i <= radius; ++i) {
                float Q = float(i * i) / (-2.0f * sigma * sigma);
                F[i] = ( float(exp(Q)) / (sigma * float(std::sqrt(2.0f * 3.14159265358979323f))));
                if (i == 0) {
                    sum += F[i];
                } else {
                    sum += 2.0f * F[i];
                }
            }
            for (int i = 0; i <= radius; ++i)
                F[i] /= sum;
            blurWeights[radius] = F;
        }
        return blurWeights[radius];
    }
    
    Framebuffer* getBlurHelper(unsigned w, unsigned h, GLenum format){
        std::array<unsigned,3> T;
        T[0]=w; T[1]=h; T[2]=format;
        if( blurHelpers.find(T) == blurHelpers.end() )
            blurHelpers[T] = new Framebuffer(w,h, 1, format );
        return blurHelpers[T];
    }
    
}; //anonymous namespace



Framebuffer::Framebuffer(int width, int height, unsigned slices, GLenum format )
{
    std::vector<Framebuffer::AttachmentInfo> v;
    v.push_back( AttachmentInfo(slices,format) );
    init(width,height, v );
}


Framebuffer::Framebuffer(int width, int height, const std::initializer_list<GLenum>& formats)
{
    std::vector<Framebuffer::AttachmentInfo> v;
    for(auto fmt: formats ){
        AttachmentInfo ai(1,fmt);
        v.push_back(ai);
    }
    init( width,height, v );
}

Framebuffer::Framebuffer(int width, int height, const std::vector<Framebuffer::AttachmentInfo>& formats )
{
    init(width,height,formats);
}


void Framebuffer::init(int width, int height, const std::vector<Framebuffer::AttachmentInfo>& formats)
{
    this->width=width;
    this->height=height;
    
    GLuint tmp[1];
    glGenFramebuffers(1,tmp);
    this->fbo = tmp[0];
    glBindFramebuffer(GL_FRAMEBUFFER, fbo );
    
    int attachmentCount=0;
    for(auto ai : formats ){
        
        //only permit formats that GL requires support for. Any combination
        //of these must be OK in an FBO according to the standard.
        //https://www.khronos.org/opengl/wiki/Image_Format#Required_formats
        //https://www.khronos.org/opengl/wiki/Framebuffer_Object
        
        GLenum efmt,etype;
        
        switch(ai.format){
            case GL_RGBA8:
            case GL_RGBA8I:
            case GL_RGBA8UI:
            case GL_RGBA16:
            case GL_RGBA16I:
            case GL_RGBA16UI:
            case GL_RGBA32F:
            case GL_RG8:
            case GL_RG8I:
            case GL_RG8UI:
            case GL_RG16:
            case GL_RG16I:
            case GL_RG16UI:
            case GL_RG32F:
            case GL_R8:
            case GL_R8I:
            case GL_R8UI:
            case GL_R16:
            case GL_R16I:
            case GL_R16UI:
            case GL_R32F:
                efmt = GL_RGBA;
                etype = GL_UNSIGNED_BYTE;
                break;
            case GL_DEPTH24_STENCIL8:
            case GL_DEPTH32F_STENCIL8:
                efmt = GL_DEPTH_STENCIL;
                etype = GL_UNSIGNED_INT_24_8;
                break;
            default:
                throw std::runtime_error("Invalid FBO texture format");
        }
        
        textures.push_back( std::make_shared<DataTexture2DArray>(
            width,height,ai.slices,ai.format,efmt,etype,nullptr));
            
        this->formats.push_back( ai.format );
        
        for(unsigned i=0;i<ai.slices;++i){
            //target, attachment, texture, mip level, layer
            glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+attachmentCount,
                textures.back()->tex, 0, i );
            drawbuffers.push_back(GL_COLOR_ATTACHMENT0+attachmentCount);
            attachmentCount++;
        }
    }
      
    if( attachmentCount > 8 ){
        std::cout << "Warning: Too many FBO attachments: " << std::to_string(attachmentCount) << "\n";
    }
    
    if( !textures.empty() )
        texture = textures[0];
    else
        texture = nullptr;
        
    depthtexture = std::make_shared<DataTexture2DArray>(width,height,1,GL_DEPTH24_STENCIL8,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,nullptr);
    glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT, depthtexture->tex, 0, 0 );
        
    int complete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( complete != GL_FRAMEBUFFER_COMPLETE ){
        throw std::runtime_error("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Framebuffer::setAsRenderTarget(bool clear){
    if( current )
        current->unsetAsRenderTarget();
    glGetIntegerv(GL_VIEWPORT,saved_viewport);
    current = this;
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glDrawBuffers((GLsizei)drawbuffers.size(),drawbuffers.data());
    glViewport(0,0,depthtexture->w,depthtexture->h);
    if( clear )
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void Framebuffer::unsetAsRenderTarget(){
    current = nullptr;
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(
        saved_viewport[0],saved_viewport[1],
        saved_viewport[2],saved_viewport[3]);
    this->texture->generateMipmap();
}
    
void Framebuffer::dump(std::string filename){
    for(unsigned j=0;j<textures.size();++j){
        auto t = textures[j];
        std::vector<char> B(t->w*t->h*4*t->slices);
        t->bind(0);
        glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, GL_UNSIGNED_BYTE, B.data());
        t->unbind(0);
        char* p = B.data();
        for(unsigned i=0;i<t->slices;++i){
            Image img(t->w,t->h,"RGBA8");
            std::memcpy( img.pixels(), p, t->w*t->h*4);
            p += t->w*t->h*4;
            std::string fn = filename+"-texture-"+std::to_string(j)+"-slice-"+std::to_string(i)+".png";
            img.writePng(fn);
            std::cout << "Wrote " << fn << "\n";
        }
    }
}
   
void Framebuffer::blur(unsigned textureIndex, unsigned slice, int radius, float multiplier){
    
         
    if( textureIndex >= (unsigned) textures.size() )
        throw std::runtime_error("Bad textureIndex: Should be between 0 and "+std::to_string(textures.size()-1));
   
    if( slice >= textures[textureIndex]->slices )
        throw std::runtime_error("Bad slice: Should be between 0 and "+std::to_string(textures[textureIndex]->slices-1));
   
    GLenum fmt = formats[textureIndex];
    
    if( radius <= 0 )
        throw std::runtime_error("Bad radius for blur: "+std::to_string(radius) );
        
    
    if( blurProgram == nullptr ){
        std::array< std::pair<std::string,std::string> ,6> tmp;
        tmp[0] = std::make_pair("blurvs",blurvs);
        tmp[4] = std::make_pair("blurfs",blurfs);
        blurProgram = new Program( tmp );
    }
    
    if( fsquad == nullptr )
        fsquad = new FullscreenQuad();

    auto helper = getBlurHelper(this->width, this->height, fmt);

    auto oldProg = Program::current;
    auto oldfbo = Framebuffer::current;
    auto depthEnabled = glIsEnabled(GL_DEPTH_TEST);

    //don't affect or check depth buffers
    glDisable(GL_DEPTH_TEST);
    
    blurProgram->use();
    
    //set constants for blur operations
    Program::setUniform("blurMultiplier",  multiplier );
    Program::setUniform("blurWeights[0]", computeBlurWeights(radius) );
    Program::setUniform("blurRadius", radius );
    
    //setup the read to be from the desired texture and slice
    Program::setUniform("blurSourceSlice", slice );
    this->textures[textureIndex]->bind(0);
    
    //draw to helper FBO. This will respect the stencil test,
    //so we don't clear the buffers
    helper->setAsRenderTarget(false);
    
    //draw full screen quad, doing horizontal blurring
    Program::setUniform("blurDeltas",vec2(1.0f/float(texture->w),0.0f));
    fsquad->draw();
    
    //read from helper FBO, slice 0
    helper->texture->bind(0);
    Program::setUniform("blurSourceSlice",0);
    
    //draw back to this fbo, in the same slice as we read from initially
    this->setAsRenderTarget(false);
    GLenum tmp = GL_COLOR_ATTACHMENT0+slice;
    glDrawBuffers(1,&tmp);
    
    //reset the multiplier so we don't do double-brightening
    Program::setUniform("blurMultiplier",  1.0f );
    
    //vertical blur
    Program::setUniform("blurDeltas",vec2(0,1.0f/float(texture->h)));
    fsquad->draw();
    
    //done with the render. Put everything back the way we found it.
    this->unsetAsRenderTarget();
    helper->texture->unbind(0);
    
    if( oldProg )
        oldProg->use();
        
    if( oldfbo )
        oldfbo->setAsRenderTarget(false);
    else{
        GLenum tmp = GL_BACK_LEFT;
        glDrawBuffers(1,&tmp);
    }
    if( depthEnabled )
        glEnable(GL_DEPTH_TEST);
        
}

void Framebuffer::copyTo(Framebuffer& otherFBO)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, otherFBO.fbo);
    glBlitFramebuffer(
        0, 0,                               //Source Start X, Y
        width, height,                      //Source End X, Y
        0, 0,                               //Destination Start X, Y
        otherFBO.width, otherFBO.height,    //Destination End X, Y
        GL_COLOR_BUFFER_BIT,                //Mask
        GL_NEAREST);                        //Filter
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //GLenum error = glGetError();
}

Framebuffer* Framebuffer::current = nullptr;

Framebuffer::AttachmentInfo::AttachmentInfo( unsigned count, GLenum fmt )
{
    this->format = fmt;
    this->slices = count;
    if(  count > 16 )
        throw std::runtime_error("Bad count for attachments");
}

Framebuffer::~Framebuffer(){
    glDeleteFramebuffers(1,&fbo);
}
