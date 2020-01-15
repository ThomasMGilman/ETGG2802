/*Data from gl.xml, which has this copyright:

#Copyright (c) 2013-2016 The Khronos Group Inc.
#
#Permission is hereby granted, free of charge, to any person obtaining a
#copy of this software and/or associated documentation files (the
#"Materials"), to deal in the Materials without restriction, including
#without limitation the rights to use, copy, modify, merge, publish,
#distribute, sublicense, and/or sell copies of the Materials, and to
#permit persons to whom the Materials are furnished to do so, subject to
#the following conditions:
#
#The above copyright notice and this permission notice shall be included
#in all copies or substantial portions of the Materials.
#
#THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
#TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
#MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
#
#------------------------------------------------------------------------
#
#This file, gl.xml, is the OpenGL and OpenGL API Registry. The older
#".spec" file format has been retired and will no longer be updated with
#new extensions and API versions. The canonical version of the registry,
#together with documentation, schema, and Python generator scripts used
#to generate C header files for OpenGL and OpenGL ES, can always be found
#in the Khronos Registry at
#        http://www.opengl.org/registry/
#    
*/
 
#ifdef _MSC_VER
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include "glfuncs.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <forward_list>

using namespace std;

namespace {
    void* mygetprocaddr(const char* funcname){
        void* x = 0;
        
    #ifdef _MSC_VER
        static HMODULE gllib = 0;
        if (!gllib) {
            gllib = LoadLibraryA("opengl32.dll");
            if (!gllib)
                throw runtime_error("Cannot load GL library");
        }
        typedef void* (__stdcall *GLT)(LPCSTR);
        static GLT wglgpa;    
        if( !wglgpa ){
            wglgpa = (GLT) GetProcAddress(gllib,"wglGetProcAddress");
            if(!wglgpa)
                throw runtime_error("Cannot find wglGetProcAddress");
        }
        
        x = (void*) wglgpa(funcname);
        
        if (!x || x == (void*)1 || x == (void*)2 || x == (void*)3 || x == (void*)-1)
            x = 0;
            
        if(!x)
            x = (void*)GetProcAddress(gllib, funcname);
            
    #else   
        static void* gllib = 0;
        if(!gllib){
            gllib = dlopen("libGL.so", RTLD_NOW);
            if (!gllib)
                throw runtime_error("Cannot load GL library");
        }
        x = dlsym(gllib, funcname);
    #endif

        if (!x)
            throw runtime_error(string("Could not load function ")+funcname);
            
        return x;
      
    }

    std::forward_list<gl::wildcardHookFunction> wildcardHooks;
    std::forward_list<gl::wildcardHookFunction> drawCallHooks;

}   //anonymous namespace

namespace gl {
    void addWildcardHook( wildcardHookFunction wf ){
        wildcardHooks.push_front(wf);
    }
    void removeWildcardHook( wildcardHookFunction wf ){
        wildcardHooks.remove(wf);
    }

    void addDrawCallHook( wildcardHookFunction wf ){
        drawCallHooks.push_front(wf);
    }
    void removeDrawCallHook( wildcardHookFunction wf ){
        drawCallHooks.remove(wf);
    }
}; //gl namespace

void glActiveShaderProgram ( GLuint pipeline_, GLuint program_ ){
    static PFNGLACTIVESHADERPROGRAMPROC F;
    if(!F)
        F = ( PFNGLACTIVESHADERPROGRAMPROC ) mygetprocaddr("glActiveShaderProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glActiveShaderProgram",params);
    }
    F(pipeline_, program_);
    return;
}
void glActiveTexture ( GLenum texture_ ){
    static PFNGLACTIVETEXTUREPROC F;
    if(!F)
        F = ( PFNGLACTIVETEXTUREPROC ) mygetprocaddr("glActiveTexture");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        for(auto& it : wildcardHooks )
            it("glActiveTexture",params);
    }
    F(texture_);
    return;
}
void glAttachShader ( GLuint program_, GLuint shader_ ){
    static PFNGLATTACHSHADERPROC F;
    if(!F)
        F = ( PFNGLATTACHSHADERPROC ) mygetprocaddr("glAttachShader");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shader_ );
        for(auto& it : wildcardHooks )
            it("glAttachShader",params);
    }
    F(program_, shader_);
    return;
}
void glBeginConditionalRender ( GLuint id_, GLenum mode_ ){
    static PFNGLBEGINCONDITIONALRENDERPROC F;
    if(!F)
        F = ( PFNGLBEGINCONDITIONALRENDERPROC ) mygetprocaddr("glBeginConditionalRender");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glBeginConditionalRender",params);
    }
    F(id_, mode_);
    return;
}
void glBeginQuery ( GLenum target_, GLuint id_ ){
    static PFNGLBEGINQUERYPROC F;
    if(!F)
        F = ( PFNGLBEGINQUERYPROC ) mygetprocaddr("glBeginQuery");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) id_ );
        for(auto& it : wildcardHooks )
            it("glBeginQuery",params);
    }
    F(target_, id_);
    return;
}
void glBeginQueryIndexed ( GLenum target_, GLuint index_, GLuint id_ ){
    static PFNGLBEGINQUERYINDEXEDPROC F;
    if(!F)
        F = ( PFNGLBEGINQUERYINDEXEDPROC ) mygetprocaddr("glBeginQueryIndexed");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) id_ );
        for(auto& it : wildcardHooks )
            it("glBeginQueryIndexed",params);
    }
    F(target_, index_, id_);
    return;
}
void glBeginTransformFeedback ( GLenum primitiveMode_ ){
    static PFNGLBEGINTRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLBEGINTRANSFORMFEEDBACKPROC ) mygetprocaddr("glBeginTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) primitiveMode_ );
        for(auto& it : wildcardHooks )
            it("glBeginTransformFeedback",params);
    }
    F(primitiveMode_);
    return;
}
void glBindAttribLocation ( GLuint program_, GLuint index_, const GLchar * name_ ){
    static PFNGLBINDATTRIBLOCATIONPROC F;
    if(!F)
        F = ( PFNGLBINDATTRIBLOCATIONPROC ) mygetprocaddr("glBindAttribLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glBindAttribLocation",params);
    }
    F(program_, index_, name_);
    return;
}
void glBindBuffer ( GLenum target_, GLuint buffer_ ){
    static PFNGLBINDBUFFERPROC F;
    if(!F)
        F = ( PFNGLBINDBUFFERPROC ) mygetprocaddr("glBindBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glBindBuffer",params);
    }
    F(target_, buffer_);
    return;
}
void glBindBufferBase ( GLenum target_, GLuint index_, GLuint buffer_ ){
    static PFNGLBINDBUFFERBASEPROC F;
    if(!F)
        F = ( PFNGLBINDBUFFERBASEPROC ) mygetprocaddr("glBindBufferBase");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glBindBufferBase",params);
    }
    F(target_, index_, buffer_);
    return;
}
void glBindBufferRange ( GLenum target_, GLuint index_, GLuint buffer_, GLintptr offset_, GLsizeiptr size_ ){
    static PFNGLBINDBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLBINDBUFFERRANGEPROC ) mygetprocaddr("glBindBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glBindBufferRange",params);
    }
    F(target_, index_, buffer_, offset_, size_);
    return;
}
void glBindBuffersBase ( GLenum target_, GLuint first_, GLsizei count_, const GLuint * buffers_ ){
    static PFNGLBINDBUFFERSBASEPROC F;
    if(!F)
        F = ( PFNGLBINDBUFFERSBASEPROC ) mygetprocaddr("glBindBuffersBase");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) buffers_ );
        for(auto& it : wildcardHooks )
            it("glBindBuffersBase",params);
    }
    F(target_, first_, count_, buffers_);
    return;
}
void glBindBuffersRange ( GLenum target_, GLuint first_, GLsizei count_, const GLuint * buffers_, const GLintptr * offsets_, const GLsizeiptr * sizes_ ){
    static PFNGLBINDBUFFERSRANGEPROC F;
    if(!F)
        F = ( PFNGLBINDBUFFERSRANGEPROC ) mygetprocaddr("glBindBuffersRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) buffers_ );
        params.push_back( (uint64_t) offsets_ );
        params.push_back( (uint64_t) sizes_ );
        for(auto& it : wildcardHooks )
            it("glBindBuffersRange",params);
    }
    F(target_, first_, count_, buffers_, offsets_, sizes_);
    return;
}
void glBindFragDataLocation ( GLuint program_, GLuint color_, const GLchar * name_ ){
    static PFNGLBINDFRAGDATALOCATIONPROC F;
    if(!F)
        F = ( PFNGLBINDFRAGDATALOCATIONPROC ) mygetprocaddr("glBindFragDataLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) color_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glBindFragDataLocation",params);
    }
    F(program_, color_, name_);
    return;
}
void glBindFragDataLocationIndexed ( GLuint program_, GLuint colorNumber_, GLuint index_, const GLchar * name_ ){
    static PFNGLBINDFRAGDATALOCATIONINDEXEDPROC F;
    if(!F)
        F = ( PFNGLBINDFRAGDATALOCATIONINDEXEDPROC ) mygetprocaddr("glBindFragDataLocationIndexed");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) colorNumber_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glBindFragDataLocationIndexed",params);
    }
    F(program_, colorNumber_, index_, name_);
    return;
}
void glBindFramebuffer ( GLenum target_, GLuint framebuffer_ ){
    static PFNGLBINDFRAMEBUFFERPROC F;
    if(!F)
        F = ( PFNGLBINDFRAMEBUFFERPROC ) mygetprocaddr("glBindFramebuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) framebuffer_ );
        for(auto& it : wildcardHooks )
            it("glBindFramebuffer",params);
    }
    F(target_, framebuffer_);
    return;
}
void glBindImageTexture ( GLuint unit_, GLuint texture_, GLint level_, GLboolean layered_, GLint layer_, GLenum access_, GLenum format_ ){
    static PFNGLBINDIMAGETEXTUREPROC F;
    if(!F)
        F = ( PFNGLBINDIMAGETEXTUREPROC ) mygetprocaddr("glBindImageTexture");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) unit_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) layered_ );
        params.push_back( (uint64_t) layer_ );
        params.push_back( (uint64_t) access_ );
        params.push_back( (uint64_t) format_ );
        for(auto& it : wildcardHooks )
            it("glBindImageTexture",params);
    }
    F(unit_, texture_, level_, layered_, layer_, access_, format_);
    return;
}
void glBindImageTextures ( GLuint first_, GLsizei count_, const GLuint * textures_ ){
    static PFNGLBINDIMAGETEXTURESPROC F;
    if(!F)
        F = ( PFNGLBINDIMAGETEXTURESPROC ) mygetprocaddr("glBindImageTextures");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) textures_ );
        for(auto& it : wildcardHooks )
            it("glBindImageTextures",params);
    }
    F(first_, count_, textures_);
    return;
}
void glBindProgramPipeline ( GLuint pipeline_ ){
    static PFNGLBINDPROGRAMPIPELINEPROC F;
    if(!F)
        F = ( PFNGLBINDPROGRAMPIPELINEPROC ) mygetprocaddr("glBindProgramPipeline");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        for(auto& it : wildcardHooks )
            it("glBindProgramPipeline",params);
    }
    F(pipeline_);
    return;
}
void glBindRenderbuffer ( GLenum target_, GLuint renderbuffer_ ){
    static PFNGLBINDRENDERBUFFERPROC F;
    if(!F)
        F = ( PFNGLBINDRENDERBUFFERPROC ) mygetprocaddr("glBindRenderbuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) renderbuffer_ );
        for(auto& it : wildcardHooks )
            it("glBindRenderbuffer",params);
    }
    F(target_, renderbuffer_);
    return;
}
void glBindSampler ( GLuint unit_, GLuint sampler_ ){
    static PFNGLBINDSAMPLERPROC F;
    if(!F)
        F = ( PFNGLBINDSAMPLERPROC ) mygetprocaddr("glBindSampler");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) unit_ );
        params.push_back( (uint64_t) sampler_ );
        for(auto& it : wildcardHooks )
            it("glBindSampler",params);
    }
    F(unit_, sampler_);
    return;
}
void glBindSamplers ( GLuint first_, GLsizei count_, const GLuint * samplers_ ){
    static PFNGLBINDSAMPLERSPROC F;
    if(!F)
        F = ( PFNGLBINDSAMPLERSPROC ) mygetprocaddr("glBindSamplers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) samplers_ );
        for(auto& it : wildcardHooks )
            it("glBindSamplers",params);
    }
    F(first_, count_, samplers_);
    return;
}
void glBindTexture ( GLenum target_, GLuint texture_ ){
    static PFNGLBINDTEXTUREPROC F;
    if(!F)
        F = ( PFNGLBINDTEXTUREPROC ) mygetprocaddr("glBindTexture");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) texture_ );
        for(auto& it : wildcardHooks )
            it("glBindTexture",params);
    }
    F(target_, texture_);
    return;
}
void glBindTextureUnit ( GLuint unit_, GLuint texture_ ){
    static PFNGLBINDTEXTUREUNITPROC F;
    if(!F)
        F = ( PFNGLBINDTEXTUREUNITPROC ) mygetprocaddr("glBindTextureUnit");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) unit_ );
        params.push_back( (uint64_t) texture_ );
        for(auto& it : wildcardHooks )
            it("glBindTextureUnit",params);
    }
    F(unit_, texture_);
    return;
}
void glBindTextures ( GLuint first_, GLsizei count_, const GLuint * textures_ ){
    static PFNGLBINDTEXTURESPROC F;
    if(!F)
        F = ( PFNGLBINDTEXTURESPROC ) mygetprocaddr("glBindTextures");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) textures_ );
        for(auto& it : wildcardHooks )
            it("glBindTextures",params);
    }
    F(first_, count_, textures_);
    return;
}
void glBindTransformFeedback ( GLenum target_, GLuint id_ ){
    static PFNGLBINDTRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLBINDTRANSFORMFEEDBACKPROC ) mygetprocaddr("glBindTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) id_ );
        for(auto& it : wildcardHooks )
            it("glBindTransformFeedback",params);
    }
    F(target_, id_);
    return;
}
void glBindVertexArray ( GLuint array_ ){
    static PFNGLBINDVERTEXARRAYPROC F;
    if(!F)
        F = ( PFNGLBINDVERTEXARRAYPROC ) mygetprocaddr("glBindVertexArray");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) array_ );
        for(auto& it : wildcardHooks )
            it("glBindVertexArray",params);
    }
    F(array_);
    return;
}
void glBindVertexBuffer ( GLuint bindingindex_, GLuint buffer_, GLintptr offset_, GLsizei stride_ ){
    static PFNGLBINDVERTEXBUFFERPROC F;
    if(!F)
        F = ( PFNGLBINDVERTEXBUFFERPROC ) mygetprocaddr("glBindVertexBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) bindingindex_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) stride_ );
        for(auto& it : wildcardHooks )
            it("glBindVertexBuffer",params);
    }
    F(bindingindex_, buffer_, offset_, stride_);
    return;
}
void glBindVertexBuffers ( GLuint first_, GLsizei count_, const GLuint * buffers_, const GLintptr * offsets_, const GLsizei * strides_ ){
    static PFNGLBINDVERTEXBUFFERSPROC F;
    if(!F)
        F = ( PFNGLBINDVERTEXBUFFERSPROC ) mygetprocaddr("glBindVertexBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) buffers_ );
        params.push_back( (uint64_t) offsets_ );
        params.push_back( (uint64_t) strides_ );
        for(auto& it : wildcardHooks )
            it("glBindVertexBuffers",params);
    }
    F(first_, count_, buffers_, offsets_, strides_);
    return;
}
void glBlendColor ( GLfloat red_, GLfloat green_, GLfloat blue_, GLfloat alpha_ ){
    static PFNGLBLENDCOLORPROC F;
    if(!F)
        F = ( PFNGLBLENDCOLORPROC ) mygetprocaddr("glBlendColor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) red_ );
        params.push_back( (uint64_t) green_ );
        params.push_back( (uint64_t) blue_ );
        params.push_back( (uint64_t) alpha_ );
        for(auto& it : wildcardHooks )
            it("glBlendColor",params);
    }
    F(red_, green_, blue_, alpha_);
    return;
}
void glBlendEquation ( GLenum mode_ ){
    static PFNGLBLENDEQUATIONPROC F;
    if(!F)
        F = ( PFNGLBLENDEQUATIONPROC ) mygetprocaddr("glBlendEquation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glBlendEquation",params);
    }
    F(mode_);
    return;
}
void glBlendEquationSeparate ( GLenum modeRGB_, GLenum modeAlpha_ ){
    static PFNGLBLENDEQUATIONSEPARATEPROC F;
    if(!F)
        F = ( PFNGLBLENDEQUATIONSEPARATEPROC ) mygetprocaddr("glBlendEquationSeparate");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) modeRGB_ );
        params.push_back( (uint64_t) modeAlpha_ );
        for(auto& it : wildcardHooks )
            it("glBlendEquationSeparate",params);
    }
    F(modeRGB_, modeAlpha_);
    return;
}
void glBlendEquationSeparatei ( GLuint buf_, GLenum modeRGB_, GLenum modeAlpha_ ){
    static PFNGLBLENDEQUATIONSEPARATEIPROC F;
    if(!F)
        F = ( PFNGLBLENDEQUATIONSEPARATEIPROC ) mygetprocaddr("glBlendEquationSeparatei");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buf_ );
        params.push_back( (uint64_t) modeRGB_ );
        params.push_back( (uint64_t) modeAlpha_ );
        for(auto& it : wildcardHooks )
            it("glBlendEquationSeparatei",params);
    }
    F(buf_, modeRGB_, modeAlpha_);
    return;
}
void glBlendEquationi ( GLuint buf_, GLenum mode_ ){
    static PFNGLBLENDEQUATIONIPROC F;
    if(!F)
        F = ( PFNGLBLENDEQUATIONIPROC ) mygetprocaddr("glBlendEquationi");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buf_ );
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glBlendEquationi",params);
    }
    F(buf_, mode_);
    return;
}
void glBlendFunc ( GLenum sfactor_, GLenum dfactor_ ){
    static PFNGLBLENDFUNCPROC F;
    if(!F)
        F = ( PFNGLBLENDFUNCPROC ) mygetprocaddr("glBlendFunc");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sfactor_ );
        params.push_back( (uint64_t) dfactor_ );
        for(auto& it : wildcardHooks )
            it("glBlendFunc",params);
    }
    F(sfactor_, dfactor_);
    return;
}
void glBlendFuncSeparate ( GLenum sfactorRGB_, GLenum dfactorRGB_, GLenum sfactorAlpha_, GLenum dfactorAlpha_ ){
    static PFNGLBLENDFUNCSEPARATEPROC F;
    if(!F)
        F = ( PFNGLBLENDFUNCSEPARATEPROC ) mygetprocaddr("glBlendFuncSeparate");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sfactorRGB_ );
        params.push_back( (uint64_t) dfactorRGB_ );
        params.push_back( (uint64_t) sfactorAlpha_ );
        params.push_back( (uint64_t) dfactorAlpha_ );
        for(auto& it : wildcardHooks )
            it("glBlendFuncSeparate",params);
    }
    F(sfactorRGB_, dfactorRGB_, sfactorAlpha_, dfactorAlpha_);
    return;
}
void glBlendFuncSeparatei ( GLuint buf_, GLenum srcRGB_, GLenum dstRGB_, GLenum srcAlpha_, GLenum dstAlpha_ ){
    static PFNGLBLENDFUNCSEPARATEIPROC F;
    if(!F)
        F = ( PFNGLBLENDFUNCSEPARATEIPROC ) mygetprocaddr("glBlendFuncSeparatei");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buf_ );
        params.push_back( (uint64_t) srcRGB_ );
        params.push_back( (uint64_t) dstRGB_ );
        params.push_back( (uint64_t) srcAlpha_ );
        params.push_back( (uint64_t) dstAlpha_ );
        for(auto& it : wildcardHooks )
            it("glBlendFuncSeparatei",params);
    }
    F(buf_, srcRGB_, dstRGB_, srcAlpha_, dstAlpha_);
    return;
}
void glBlendFunci ( GLuint buf_, GLenum src_, GLenum dst_ ){
    static PFNGLBLENDFUNCIPROC F;
    if(!F)
        F = ( PFNGLBLENDFUNCIPROC ) mygetprocaddr("glBlendFunci");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buf_ );
        params.push_back( (uint64_t) src_ );
        params.push_back( (uint64_t) dst_ );
        for(auto& it : wildcardHooks )
            it("glBlendFunci",params);
    }
    F(buf_, src_, dst_);
    return;
}
void glBlitFramebuffer ( GLint srcX0_, GLint srcY0_, GLint srcX1_, GLint srcY1_, GLint dstX0_, GLint dstY0_, GLint dstX1_, GLint dstY1_, GLbitfield mask_, GLenum filter_ ){
    static PFNGLBLITFRAMEBUFFERPROC F;
    if(!F)
        F = ( PFNGLBLITFRAMEBUFFERPROC ) mygetprocaddr("glBlitFramebuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) srcX0_ );
        params.push_back( (uint64_t) srcY0_ );
        params.push_back( (uint64_t) srcX1_ );
        params.push_back( (uint64_t) srcY1_ );
        params.push_back( (uint64_t) dstX0_ );
        params.push_back( (uint64_t) dstY0_ );
        params.push_back( (uint64_t) dstX1_ );
        params.push_back( (uint64_t) dstY1_ );
        params.push_back( (uint64_t) mask_ );
        params.push_back( (uint64_t) filter_ );
        for(auto& it : wildcardHooks )
            it("glBlitFramebuffer",params);
    }
    F(srcX0_, srcY0_, srcX1_, srcY1_, dstX0_, dstY0_, dstX1_, dstY1_, mask_, filter_);
    return;
}
void glBlitNamedFramebuffer ( GLuint readFramebuffer_, GLuint drawFramebuffer_, GLint srcX0_, GLint srcY0_, GLint srcX1_, GLint srcY1_, GLint dstX0_, GLint dstY0_, GLint dstX1_, GLint dstY1_, GLbitfield mask_, GLenum filter_ ){
    static PFNGLBLITNAMEDFRAMEBUFFERPROC F;
    if(!F)
        F = ( PFNGLBLITNAMEDFRAMEBUFFERPROC ) mygetprocaddr("glBlitNamedFramebuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) readFramebuffer_ );
        params.push_back( (uint64_t) drawFramebuffer_ );
        params.push_back( (uint64_t) srcX0_ );
        params.push_back( (uint64_t) srcY0_ );
        params.push_back( (uint64_t) srcX1_ );
        params.push_back( (uint64_t) srcY1_ );
        params.push_back( (uint64_t) dstX0_ );
        params.push_back( (uint64_t) dstY0_ );
        params.push_back( (uint64_t) dstX1_ );
        params.push_back( (uint64_t) dstY1_ );
        params.push_back( (uint64_t) mask_ );
        params.push_back( (uint64_t) filter_ );
        for(auto& it : wildcardHooks )
            it("glBlitNamedFramebuffer",params);
    }
    F(readFramebuffer_, drawFramebuffer_, srcX0_, srcY0_, srcX1_, srcY1_, dstX0_, dstY0_, dstX1_, dstY1_, mask_, filter_);
    return;
}
void glBufferData ( GLenum target_, GLsizeiptr size_, const void * data_, GLenum usage_ ){
    static PFNGLBUFFERDATAPROC F;
    if(!F)
        F = ( PFNGLBUFFERDATAPROC ) mygetprocaddr("glBufferData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        params.push_back( (uint64_t) usage_ );
        for(auto& it : wildcardHooks )
            it("glBufferData",params);
    }
    F(target_, size_, data_, usage_);
    return;
}
void glBufferStorage ( GLenum target_, GLsizeiptr size_, const void * data_, GLbitfield flags_ ){
    static PFNGLBUFFERSTORAGEPROC F;
    if(!F)
        F = ( PFNGLBUFFERSTORAGEPROC ) mygetprocaddr("glBufferStorage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        params.push_back( (uint64_t) flags_ );
        for(auto& it : wildcardHooks )
            it("glBufferStorage",params);
    }
    F(target_, size_, data_, flags_);
    return;
}
void glBufferSubData ( GLenum target_, GLintptr offset_, GLsizeiptr size_, const void * data_ ){
    static PFNGLBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLBUFFERSUBDATAPROC ) mygetprocaddr("glBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glBufferSubData",params);
    }
    F(target_, offset_, size_, data_);
    return;
}
GLenum glCheckFramebufferStatus ( GLenum target_ ){
    static PFNGLCHECKFRAMEBUFFERSTATUSPROC F;
    if(!F)
        F = ( PFNGLCHECKFRAMEBUFFERSTATUSPROC ) mygetprocaddr("glCheckFramebufferStatus");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        for(auto& it : wildcardHooks )
            it("glCheckFramebufferStatus",params);
    }
    GLenum retval = F(target_);
    return retval;
}
GLenum glCheckNamedFramebufferStatus ( GLuint framebuffer_, GLenum target_ ){
    static PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC F;
    if(!F)
        F = ( PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC ) mygetprocaddr("glCheckNamedFramebufferStatus");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) target_ );
        for(auto& it : wildcardHooks )
            it("glCheckNamedFramebufferStatus",params);
    }
    GLenum retval = F(framebuffer_, target_);
    return retval;
}
void glClampColor ( GLenum target_, GLenum clamp_ ){
    static PFNGLCLAMPCOLORPROC F;
    if(!F)
        F = ( PFNGLCLAMPCOLORPROC ) mygetprocaddr("glClampColor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) clamp_ );
        for(auto& it : wildcardHooks )
            it("glClampColor",params);
    }
    F(target_, clamp_);
    return;
}
void glClear ( GLbitfield mask_ ){
    static PFNGLCLEARPROC F;
    if(!F)
        F = ( PFNGLCLEARPROC ) mygetprocaddr("glClear");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mask_ );
        for(auto& it : wildcardHooks )
            it("glClear",params);
    }
    F(mask_);
    return;
}
void glClearBufferData ( GLenum target_, GLenum internalformat_, GLenum format_, GLenum type_, const void * data_ ){
    static PFNGLCLEARBUFFERDATAPROC F;
    if(!F)
        F = ( PFNGLCLEARBUFFERDATAPROC ) mygetprocaddr("glClearBufferData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glClearBufferData",params);
    }
    F(target_, internalformat_, format_, type_, data_);
    return;
}
void glClearBufferSubData ( GLenum target_, GLenum internalformat_, GLintptr offset_, GLsizeiptr size_, GLenum format_, GLenum type_, const void * data_ ){
    static PFNGLCLEARBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLCLEARBUFFERSUBDATAPROC ) mygetprocaddr("glClearBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glClearBufferSubData",params);
    }
    F(target_, internalformat_, offset_, size_, format_, type_, data_);
    return;
}
void glClearBufferfi ( GLenum buffer_, GLint drawbuffer_, GLfloat depth_, GLint stencil_ ){
    static PFNGLCLEARBUFFERFIPROC F;
    if(!F)
        F = ( PFNGLCLEARBUFFERFIPROC ) mygetprocaddr("glClearBufferfi");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) stencil_ );
        for(auto& it : wildcardHooks )
            it("glClearBufferfi",params);
    }
    F(buffer_, drawbuffer_, depth_, stencil_);
    return;
}
void glClearBufferfv ( GLenum buffer_, GLint drawbuffer_, const GLfloat * value_ ){
    static PFNGLCLEARBUFFERFVPROC F;
    if(!F)
        F = ( PFNGLCLEARBUFFERFVPROC ) mygetprocaddr("glClearBufferfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glClearBufferfv",params);
    }
    F(buffer_, drawbuffer_, value_);
    return;
}
void glClearBufferiv ( GLenum buffer_, GLint drawbuffer_, const GLint * value_ ){
    static PFNGLCLEARBUFFERIVPROC F;
    if(!F)
        F = ( PFNGLCLEARBUFFERIVPROC ) mygetprocaddr("glClearBufferiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glClearBufferiv",params);
    }
    F(buffer_, drawbuffer_, value_);
    return;
}
void glClearBufferuiv ( GLenum buffer_, GLint drawbuffer_, const GLuint * value_ ){
    static PFNGLCLEARBUFFERUIVPROC F;
    if(!F)
        F = ( PFNGLCLEARBUFFERUIVPROC ) mygetprocaddr("glClearBufferuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glClearBufferuiv",params);
    }
    F(buffer_, drawbuffer_, value_);
    return;
}
void glClearColor ( GLfloat red_, GLfloat green_, GLfloat blue_, GLfloat alpha_ ){
    static PFNGLCLEARCOLORPROC F;
    if(!F)
        F = ( PFNGLCLEARCOLORPROC ) mygetprocaddr("glClearColor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) red_ );
        params.push_back( (uint64_t) green_ );
        params.push_back( (uint64_t) blue_ );
        params.push_back( (uint64_t) alpha_ );
        for(auto& it : wildcardHooks )
            it("glClearColor",params);
    }
    F(red_, green_, blue_, alpha_);
    return;
}
void glClearDepth ( GLdouble depth_ ){
    static PFNGLCLEARDEPTHPROC F;
    if(!F)
        F = ( PFNGLCLEARDEPTHPROC ) mygetprocaddr("glClearDepth");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) depth_ );
        for(auto& it : wildcardHooks )
            it("glClearDepth",params);
    }
    F(depth_);
    return;
}
void glClearDepthf ( GLfloat d_ ){
    static PFNGLCLEARDEPTHFPROC F;
    if(!F)
        F = ( PFNGLCLEARDEPTHFPROC ) mygetprocaddr("glClearDepthf");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) d_ );
        for(auto& it : wildcardHooks )
            it("glClearDepthf",params);
    }
    F(d_);
    return;
}
void glClearNamedBufferData ( GLuint buffer_, GLenum internalformat_, GLenum format_, GLenum type_, const void * data_ ){
    static PFNGLCLEARNAMEDBUFFERDATAPROC F;
    if(!F)
        F = ( PFNGLCLEARNAMEDBUFFERDATAPROC ) mygetprocaddr("glClearNamedBufferData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glClearNamedBufferData",params);
    }
    F(buffer_, internalformat_, format_, type_, data_);
    return;
}
void glClearNamedBufferSubData ( GLuint buffer_, GLenum internalformat_, GLintptr offset_, GLsizeiptr size_, GLenum format_, GLenum type_, const void * data_ ){
    static PFNGLCLEARNAMEDBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLCLEARNAMEDBUFFERSUBDATAPROC ) mygetprocaddr("glClearNamedBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glClearNamedBufferSubData",params);
    }
    F(buffer_, internalformat_, offset_, size_, format_, type_, data_);
    return;
}
void glClearNamedFramebufferfi ( GLuint framebuffer_, GLenum buffer_, GLint drawbuffer_, GLfloat depth_, GLint stencil_ ){
    static PFNGLCLEARNAMEDFRAMEBUFFERFIPROC F;
    if(!F)
        F = ( PFNGLCLEARNAMEDFRAMEBUFFERFIPROC ) mygetprocaddr("glClearNamedFramebufferfi");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) stencil_ );
        for(auto& it : wildcardHooks )
            it("glClearNamedFramebufferfi",params);
    }
    F(framebuffer_, buffer_, drawbuffer_, depth_, stencil_);
    return;
}
void glClearNamedFramebufferfv ( GLuint framebuffer_, GLenum buffer_, GLint drawbuffer_, const GLfloat * value_ ){
    static PFNGLCLEARNAMEDFRAMEBUFFERFVPROC F;
    if(!F)
        F = ( PFNGLCLEARNAMEDFRAMEBUFFERFVPROC ) mygetprocaddr("glClearNamedFramebufferfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glClearNamedFramebufferfv",params);
    }
    F(framebuffer_, buffer_, drawbuffer_, value_);
    return;
}
void glClearNamedFramebufferiv ( GLuint framebuffer_, GLenum buffer_, GLint drawbuffer_, const GLint * value_ ){
    static PFNGLCLEARNAMEDFRAMEBUFFERIVPROC F;
    if(!F)
        F = ( PFNGLCLEARNAMEDFRAMEBUFFERIVPROC ) mygetprocaddr("glClearNamedFramebufferiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glClearNamedFramebufferiv",params);
    }
    F(framebuffer_, buffer_, drawbuffer_, value_);
    return;
}
void glClearNamedFramebufferuiv ( GLuint framebuffer_, GLenum buffer_, GLint drawbuffer_, const GLuint * value_ ){
    static PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC F;
    if(!F)
        F = ( PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC ) mygetprocaddr("glClearNamedFramebufferuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) drawbuffer_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glClearNamedFramebufferuiv",params);
    }
    F(framebuffer_, buffer_, drawbuffer_, value_);
    return;
}
void glClearStencil ( GLint s_ ){
    static PFNGLCLEARSTENCILPROC F;
    if(!F)
        F = ( PFNGLCLEARSTENCILPROC ) mygetprocaddr("glClearStencil");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) s_ );
        for(auto& it : wildcardHooks )
            it("glClearStencil",params);
    }
    F(s_);
    return;
}
void glClearTexImage ( GLuint texture_, GLint level_, GLenum format_, GLenum type_, const void * data_ ){
    static PFNGLCLEARTEXIMAGEPROC F;
    if(!F)
        F = ( PFNGLCLEARTEXIMAGEPROC ) mygetprocaddr("glClearTexImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glClearTexImage",params);
    }
    F(texture_, level_, format_, type_, data_);
    return;
}
void glClearTexSubImage ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLenum format_, GLenum type_, const void * data_ ){
    static PFNGLCLEARTEXSUBIMAGEPROC F;
    if(!F)
        F = ( PFNGLCLEARTEXSUBIMAGEPROC ) mygetprocaddr("glClearTexSubImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glClearTexSubImage",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, format_, type_, data_);
    return;
}
GLenum glClientWaitSync ( GLsync sync_, GLbitfield flags_, GLuint64 timeout_ ){
    static PFNGLCLIENTWAITSYNCPROC F;
    if(!F)
        F = ( PFNGLCLIENTWAITSYNCPROC ) mygetprocaddr("glClientWaitSync");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sync_ );
        params.push_back( (uint64_t) flags_ );
        params.push_back( (uint64_t) timeout_ );
        for(auto& it : wildcardHooks )
            it("glClientWaitSync",params);
    }
    GLenum retval = F(sync_, flags_, timeout_);
    return retval;
}
void glClipControl ( GLenum origin_, GLenum depth_ ){
    static PFNGLCLIPCONTROLPROC F;
    if(!F)
        F = ( PFNGLCLIPCONTROLPROC ) mygetprocaddr("glClipControl");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) origin_ );
        params.push_back( (uint64_t) depth_ );
        for(auto& it : wildcardHooks )
            it("glClipControl",params);
    }
    F(origin_, depth_);
    return;
}
void glColorMask ( GLboolean red_, GLboolean green_, GLboolean blue_, GLboolean alpha_ ){
    static PFNGLCOLORMASKPROC F;
    if(!F)
        F = ( PFNGLCOLORMASKPROC ) mygetprocaddr("glColorMask");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) red_ );
        params.push_back( (uint64_t) green_ );
        params.push_back( (uint64_t) blue_ );
        params.push_back( (uint64_t) alpha_ );
        for(auto& it : wildcardHooks )
            it("glColorMask",params);
    }
    F(red_, green_, blue_, alpha_);
    return;
}
void glColorMaski ( GLuint index_, GLboolean r_, GLboolean g_, GLboolean b_, GLboolean a_ ){
    static PFNGLCOLORMASKIPROC F;
    if(!F)
        F = ( PFNGLCOLORMASKIPROC ) mygetprocaddr("glColorMaski");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) r_ );
        params.push_back( (uint64_t) g_ );
        params.push_back( (uint64_t) b_ );
        params.push_back( (uint64_t) a_ );
        for(auto& it : wildcardHooks )
            it("glColorMaski",params);
    }
    F(index_, r_, g_, b_, a_);
    return;
}
void glCompileShader ( GLuint shader_ ){
    static PFNGLCOMPILESHADERPROC F;
    if(!F)
        F = ( PFNGLCOMPILESHADERPROC ) mygetprocaddr("glCompileShader");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        for(auto& it : wildcardHooks )
            it("glCompileShader",params);
    }
    F(shader_);
    return;
}
void glCompressedTexImage1D ( GLenum target_, GLint level_, GLenum internalformat_, GLsizei width_, GLint border_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXIMAGE1DPROC ) mygetprocaddr("glCompressedTexImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) border_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTexImage1D",params);
    }
    F(target_, level_, internalformat_, width_, border_, imageSize_, data_);
    return;
}
void glCompressedTexImage2D ( GLenum target_, GLint level_, GLenum internalformat_, GLsizei width_, GLsizei height_, GLint border_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXIMAGE2DPROC ) mygetprocaddr("glCompressedTexImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) border_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTexImage2D",params);
    }
    F(target_, level_, internalformat_, width_, height_, border_, imageSize_, data_);
    return;
}
void glCompressedTexImage3D ( GLenum target_, GLint level_, GLenum internalformat_, GLsizei width_, GLsizei height_, GLsizei depth_, GLint border_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXIMAGE3DPROC ) mygetprocaddr("glCompressedTexImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) border_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTexImage3D",params);
    }
    F(target_, level_, internalformat_, width_, height_, depth_, border_, imageSize_, data_);
    return;
}
void glCompressedTexSubImage1D ( GLenum target_, GLint level_, GLint xoffset_, GLsizei width_, GLenum format_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC ) mygetprocaddr("glCompressedTexSubImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTexSubImage1D",params);
    }
    F(target_, level_, xoffset_, width_, format_, imageSize_, data_);
    return;
}
void glCompressedTexSubImage2D ( GLenum target_, GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_, GLenum format_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC ) mygetprocaddr("glCompressedTexSubImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTexSubImage2D",params);
    }
    F(target_, level_, xoffset_, yoffset_, width_, height_, format_, imageSize_, data_);
    return;
}
void glCompressedTexSubImage3D ( GLenum target_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLenum format_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC ) mygetprocaddr("glCompressedTexSubImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTexSubImage3D",params);
    }
    F(target_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, format_, imageSize_, data_);
    return;
}
void glCompressedTextureSubImage1D ( GLuint texture_, GLint level_, GLint xoffset_, GLsizei width_, GLenum format_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC ) mygetprocaddr("glCompressedTextureSubImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTextureSubImage1D",params);
    }
    F(texture_, level_, xoffset_, width_, format_, imageSize_, data_);
    return;
}
void glCompressedTextureSubImage2D ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_, GLenum format_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC ) mygetprocaddr("glCompressedTextureSubImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTextureSubImage2D",params);
    }
    F(texture_, level_, xoffset_, yoffset_, width_, height_, format_, imageSize_, data_);
    return;
}
void glCompressedTextureSubImage3D ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLenum format_, GLsizei imageSize_, const void * data_ ){
    static PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC ) mygetprocaddr("glCompressedTextureSubImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) imageSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glCompressedTextureSubImage3D",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, format_, imageSize_, data_);
    return;
}
void glCopyBufferSubData ( GLenum readTarget_, GLenum writeTarget_, GLintptr readOffset_, GLintptr writeOffset_, GLsizeiptr size_ ){
    static PFNGLCOPYBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLCOPYBUFFERSUBDATAPROC ) mygetprocaddr("glCopyBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) readTarget_ );
        params.push_back( (uint64_t) writeTarget_ );
        params.push_back( (uint64_t) readOffset_ );
        params.push_back( (uint64_t) writeOffset_ );
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glCopyBufferSubData",params);
    }
    F(readTarget_, writeTarget_, readOffset_, writeOffset_, size_);
    return;
}
void glCopyImageSubData ( GLuint srcName_, GLenum srcTarget_, GLint srcLevel_, GLint srcX_, GLint srcY_, GLint srcZ_, GLuint dstName_, GLenum dstTarget_, GLint dstLevel_, GLint dstX_, GLint dstY_, GLint dstZ_, GLsizei srcWidth_, GLsizei srcHeight_, GLsizei srcDepth_ ){
    static PFNGLCOPYIMAGESUBDATAPROC F;
    if(!F)
        F = ( PFNGLCOPYIMAGESUBDATAPROC ) mygetprocaddr("glCopyImageSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) srcName_ );
        params.push_back( (uint64_t) srcTarget_ );
        params.push_back( (uint64_t) srcLevel_ );
        params.push_back( (uint64_t) srcX_ );
        params.push_back( (uint64_t) srcY_ );
        params.push_back( (uint64_t) srcZ_ );
        params.push_back( (uint64_t) dstName_ );
        params.push_back( (uint64_t) dstTarget_ );
        params.push_back( (uint64_t) dstLevel_ );
        params.push_back( (uint64_t) dstX_ );
        params.push_back( (uint64_t) dstY_ );
        params.push_back( (uint64_t) dstZ_ );
        params.push_back( (uint64_t) srcWidth_ );
        params.push_back( (uint64_t) srcHeight_ );
        params.push_back( (uint64_t) srcDepth_ );
        for(auto& it : wildcardHooks )
            it("glCopyImageSubData",params);
    }
    F(srcName_, srcTarget_, srcLevel_, srcX_, srcY_, srcZ_, dstName_, dstTarget_, dstLevel_, dstX_, dstY_, dstZ_, srcWidth_, srcHeight_, srcDepth_);
    return;
}
void glCopyNamedBufferSubData ( GLuint readBuffer_, GLuint writeBuffer_, GLintptr readOffset_, GLintptr writeOffset_, GLsizeiptr size_ ){
    static PFNGLCOPYNAMEDBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLCOPYNAMEDBUFFERSUBDATAPROC ) mygetprocaddr("glCopyNamedBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) readBuffer_ );
        params.push_back( (uint64_t) writeBuffer_ );
        params.push_back( (uint64_t) readOffset_ );
        params.push_back( (uint64_t) writeOffset_ );
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glCopyNamedBufferSubData",params);
    }
    F(readBuffer_, writeBuffer_, readOffset_, writeOffset_, size_);
    return;
}
void glCopyTexImage1D ( GLenum target_, GLint level_, GLenum internalformat_, GLint x_, GLint y_, GLsizei width_, GLint border_ ){
    static PFNGLCOPYTEXIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXIMAGE1DPROC ) mygetprocaddr("glCopyTexImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) border_ );
        for(auto& it : wildcardHooks )
            it("glCopyTexImage1D",params);
    }
    F(target_, level_, internalformat_, x_, y_, width_, border_);
    return;
}
void glCopyTexImage2D ( GLenum target_, GLint level_, GLenum internalformat_, GLint x_, GLint y_, GLsizei width_, GLsizei height_, GLint border_ ){
    static PFNGLCOPYTEXIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXIMAGE2DPROC ) mygetprocaddr("glCopyTexImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) border_ );
        for(auto& it : wildcardHooks )
            it("glCopyTexImage2D",params);
    }
    F(target_, level_, internalformat_, x_, y_, width_, height_, border_);
    return;
}
void glCopyTexSubImage1D ( GLenum target_, GLint level_, GLint xoffset_, GLint x_, GLint y_, GLsizei width_ ){
    static PFNGLCOPYTEXSUBIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXSUBIMAGE1DPROC ) mygetprocaddr("glCopyTexSubImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        for(auto& it : wildcardHooks )
            it("glCopyTexSubImage1D",params);
    }
    F(target_, level_, xoffset_, x_, y_, width_);
    return;
}
void glCopyTexSubImage2D ( GLenum target_, GLint level_, GLint xoffset_, GLint yoffset_, GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLCOPYTEXSUBIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXSUBIMAGE2DPROC ) mygetprocaddr("glCopyTexSubImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glCopyTexSubImage2D",params);
    }
    F(target_, level_, xoffset_, yoffset_, x_, y_, width_, height_);
    return;
}
void glCopyTexSubImage3D ( GLenum target_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLCOPYTEXSUBIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXSUBIMAGE3DPROC ) mygetprocaddr("glCopyTexSubImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glCopyTexSubImage3D",params);
    }
    F(target_, level_, xoffset_, yoffset_, zoffset_, x_, y_, width_, height_);
    return;
}
void glCopyTextureSubImage1D ( GLuint texture_, GLint level_, GLint xoffset_, GLint x_, GLint y_, GLsizei width_ ){
    static PFNGLCOPYTEXTURESUBIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXTURESUBIMAGE1DPROC ) mygetprocaddr("glCopyTextureSubImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        for(auto& it : wildcardHooks )
            it("glCopyTextureSubImage1D",params);
    }
    F(texture_, level_, xoffset_, x_, y_, width_);
    return;
}
void glCopyTextureSubImage2D ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLCOPYTEXTURESUBIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXTURESUBIMAGE2DPROC ) mygetprocaddr("glCopyTextureSubImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glCopyTextureSubImage2D",params);
    }
    F(texture_, level_, xoffset_, yoffset_, x_, y_, width_, height_);
    return;
}
void glCopyTextureSubImage3D ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLCOPYTEXTURESUBIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLCOPYTEXTURESUBIMAGE3DPROC ) mygetprocaddr("glCopyTextureSubImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glCopyTextureSubImage3D",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, x_, y_, width_, height_);
    return;
}
void glCreateBuffers ( GLsizei n_, GLuint * buffers_ ){
    static PFNGLCREATEBUFFERSPROC F;
    if(!F)
        F = ( PFNGLCREATEBUFFERSPROC ) mygetprocaddr("glCreateBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) buffers_ );
        for(auto& it : wildcardHooks )
            it("glCreateBuffers",params);
    }
    F(n_, buffers_);
    return;
}
void glCreateFramebuffers ( GLsizei n_, GLuint * framebuffers_ ){
    static PFNGLCREATEFRAMEBUFFERSPROC F;
    if(!F)
        F = ( PFNGLCREATEFRAMEBUFFERSPROC ) mygetprocaddr("glCreateFramebuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) framebuffers_ );
        for(auto& it : wildcardHooks )
            it("glCreateFramebuffers",params);
    }
    F(n_, framebuffers_);
    return;
}
GLuint glCreateProgram (  ){
    static PFNGLCREATEPROGRAMPROC F;
    if(!F)
        F = ( PFNGLCREATEPROGRAMPROC ) mygetprocaddr("glCreateProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glCreateProgram",params);
    }
    GLuint retval = F();
    return retval;
}
void glCreateProgramPipelines ( GLsizei n_, GLuint * pipelines_ ){
    static PFNGLCREATEPROGRAMPIPELINESPROC F;
    if(!F)
        F = ( PFNGLCREATEPROGRAMPIPELINESPROC ) mygetprocaddr("glCreateProgramPipelines");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) pipelines_ );
        for(auto& it : wildcardHooks )
            it("glCreateProgramPipelines",params);
    }
    F(n_, pipelines_);
    return;
}
void glCreateQueries ( GLenum target_, GLsizei n_, GLuint * ids_ ){
    static PFNGLCREATEQUERIESPROC F;
    if(!F)
        F = ( PFNGLCREATEQUERIESPROC ) mygetprocaddr("glCreateQueries");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) ids_ );
        for(auto& it : wildcardHooks )
            it("glCreateQueries",params);
    }
    F(target_, n_, ids_);
    return;
}
void glCreateRenderbuffers ( GLsizei n_, GLuint * renderbuffers_ ){
    static PFNGLCREATERENDERBUFFERSPROC F;
    if(!F)
        F = ( PFNGLCREATERENDERBUFFERSPROC ) mygetprocaddr("glCreateRenderbuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) renderbuffers_ );
        for(auto& it : wildcardHooks )
            it("glCreateRenderbuffers",params);
    }
    F(n_, renderbuffers_);
    return;
}
void glCreateSamplers ( GLsizei n_, GLuint * samplers_ ){
    static PFNGLCREATESAMPLERSPROC F;
    if(!F)
        F = ( PFNGLCREATESAMPLERSPROC ) mygetprocaddr("glCreateSamplers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) samplers_ );
        for(auto& it : wildcardHooks )
            it("glCreateSamplers",params);
    }
    F(n_, samplers_);
    return;
}
GLuint glCreateShader ( GLenum type_ ){
    static PFNGLCREATESHADERPROC F;
    if(!F)
        F = ( PFNGLCREATESHADERPROC ) mygetprocaddr("glCreateShader");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) type_ );
        for(auto& it : wildcardHooks )
            it("glCreateShader",params);
    }
    GLuint retval = F(type_);
    return retval;
}
GLuint glCreateShaderProgramv ( GLenum type_, GLsizei count_, const GLchar ** strings_ ){
    static PFNGLCREATESHADERPROGRAMVPROC F;
    if(!F)
        F = ( PFNGLCREATESHADERPROGRAMVPROC ) mygetprocaddr("glCreateShaderProgramv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) strings_ );
        for(auto& it : wildcardHooks )
            it("glCreateShaderProgramv",params);
    }
    GLuint retval = F(type_, count_, strings_);
    return retval;
}
void glCreateTextures ( GLenum target_, GLsizei n_, GLuint * textures_ ){
    static PFNGLCREATETEXTURESPROC F;
    if(!F)
        F = ( PFNGLCREATETEXTURESPROC ) mygetprocaddr("glCreateTextures");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) textures_ );
        for(auto& it : wildcardHooks )
            it("glCreateTextures",params);
    }
    F(target_, n_, textures_);
    return;
}
void glCreateTransformFeedbacks ( GLsizei n_, GLuint * ids_ ){
    static PFNGLCREATETRANSFORMFEEDBACKSPROC F;
    if(!F)
        F = ( PFNGLCREATETRANSFORMFEEDBACKSPROC ) mygetprocaddr("glCreateTransformFeedbacks");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) ids_ );
        for(auto& it : wildcardHooks )
            it("glCreateTransformFeedbacks",params);
    }
    F(n_, ids_);
    return;
}
void glCreateVertexArrays ( GLsizei n_, GLuint * arrays_ ){
    static PFNGLCREATEVERTEXARRAYSPROC F;
    if(!F)
        F = ( PFNGLCREATEVERTEXARRAYSPROC ) mygetprocaddr("glCreateVertexArrays");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) arrays_ );
        for(auto& it : wildcardHooks )
            it("glCreateVertexArrays",params);
    }
    F(n_, arrays_);
    return;
}
void glCullFace ( GLenum mode_ ){
    static PFNGLCULLFACEPROC F;
    if(!F)
        F = ( PFNGLCULLFACEPROC ) mygetprocaddr("glCullFace");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glCullFace",params);
    }
    F(mode_);
    return;
}
void glDebugMessageCallback ( GLDEBUGPROC callback_, const void * userParam_ ){
    static PFNGLDEBUGMESSAGECALLBACKPROC F;
    if(!F)
        F = ( PFNGLDEBUGMESSAGECALLBACKPROC ) mygetprocaddr("glDebugMessageCallback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) callback_ );
        params.push_back( (uint64_t) userParam_ );
        for(auto& it : wildcardHooks )
            it("glDebugMessageCallback",params);
    }
    F(callback_, userParam_);
    return;
}
void glDebugMessageControl ( GLenum source_, GLenum type_, GLenum severity_, GLsizei count_, const GLuint * ids_, GLboolean enabled_ ){
    static PFNGLDEBUGMESSAGECONTROLPROC F;
    if(!F)
        F = ( PFNGLDEBUGMESSAGECONTROLPROC ) mygetprocaddr("glDebugMessageControl");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) source_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) severity_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) ids_ );
        params.push_back( (uint64_t) enabled_ );
        for(auto& it : wildcardHooks )
            it("glDebugMessageControl",params);
    }
    F(source_, type_, severity_, count_, ids_, enabled_);
    return;
}
void glDebugMessageInsert ( GLenum source_, GLenum type_, GLuint id_, GLenum severity_, GLsizei length_, const GLchar * buf_ ){
    static PFNGLDEBUGMESSAGEINSERTPROC F;
    if(!F)
        F = ( PFNGLDEBUGMESSAGEINSERTPROC ) mygetprocaddr("glDebugMessageInsert");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) source_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) severity_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) buf_ );
        for(auto& it : wildcardHooks )
            it("glDebugMessageInsert",params);
    }
    F(source_, type_, id_, severity_, length_, buf_);
    return;
}
void glDeleteBuffers ( GLsizei n_, const GLuint * buffers_ ){
    static PFNGLDELETEBUFFERSPROC F;
    if(!F)
        F = ( PFNGLDELETEBUFFERSPROC ) mygetprocaddr("glDeleteBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) buffers_ );
        for(auto& it : wildcardHooks )
            it("glDeleteBuffers",params);
    }
    F(n_, buffers_);
    return;
}
void glDeleteFramebuffers ( GLsizei n_, const GLuint * framebuffers_ ){
    static PFNGLDELETEFRAMEBUFFERSPROC F;
    if(!F)
        F = ( PFNGLDELETEFRAMEBUFFERSPROC ) mygetprocaddr("glDeleteFramebuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) framebuffers_ );
        for(auto& it : wildcardHooks )
            it("glDeleteFramebuffers",params);
    }
    F(n_, framebuffers_);
    return;
}
void glDeleteProgram ( GLuint program_ ){
    static PFNGLDELETEPROGRAMPROC F;
    if(!F)
        F = ( PFNGLDELETEPROGRAMPROC ) mygetprocaddr("glDeleteProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glDeleteProgram",params);
    }
    F(program_);
    return;
}
void glDeleteProgramPipelines ( GLsizei n_, const GLuint * pipelines_ ){
    static PFNGLDELETEPROGRAMPIPELINESPROC F;
    if(!F)
        F = ( PFNGLDELETEPROGRAMPIPELINESPROC ) mygetprocaddr("glDeleteProgramPipelines");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) pipelines_ );
        for(auto& it : wildcardHooks )
            it("glDeleteProgramPipelines",params);
    }
    F(n_, pipelines_);
    return;
}
void glDeleteQueries ( GLsizei n_, const GLuint * ids_ ){
    static PFNGLDELETEQUERIESPROC F;
    if(!F)
        F = ( PFNGLDELETEQUERIESPROC ) mygetprocaddr("glDeleteQueries");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) ids_ );
        for(auto& it : wildcardHooks )
            it("glDeleteQueries",params);
    }
    F(n_, ids_);
    return;
}
void glDeleteRenderbuffers ( GLsizei n_, const GLuint * renderbuffers_ ){
    static PFNGLDELETERENDERBUFFERSPROC F;
    if(!F)
        F = ( PFNGLDELETERENDERBUFFERSPROC ) mygetprocaddr("glDeleteRenderbuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) renderbuffers_ );
        for(auto& it : wildcardHooks )
            it("glDeleteRenderbuffers",params);
    }
    F(n_, renderbuffers_);
    return;
}
void glDeleteSamplers ( GLsizei count_, const GLuint * samplers_ ){
    static PFNGLDELETESAMPLERSPROC F;
    if(!F)
        F = ( PFNGLDELETESAMPLERSPROC ) mygetprocaddr("glDeleteSamplers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) samplers_ );
        for(auto& it : wildcardHooks )
            it("glDeleteSamplers",params);
    }
    F(count_, samplers_);
    return;
}
void glDeleteShader ( GLuint shader_ ){
    static PFNGLDELETESHADERPROC F;
    if(!F)
        F = ( PFNGLDELETESHADERPROC ) mygetprocaddr("glDeleteShader");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        for(auto& it : wildcardHooks )
            it("glDeleteShader",params);
    }
    F(shader_);
    return;
}
void glDeleteSync ( GLsync sync_ ){
    static PFNGLDELETESYNCPROC F;
    if(!F)
        F = ( PFNGLDELETESYNCPROC ) mygetprocaddr("glDeleteSync");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sync_ );
        for(auto& it : wildcardHooks )
            it("glDeleteSync",params);
    }
    F(sync_);
    return;
}
void glDeleteTextures ( GLsizei n_, const GLuint * textures_ ){
    static PFNGLDELETETEXTURESPROC F;
    if(!F)
        F = ( PFNGLDELETETEXTURESPROC ) mygetprocaddr("glDeleteTextures");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) textures_ );
        for(auto& it : wildcardHooks )
            it("glDeleteTextures",params);
    }
    F(n_, textures_);
    return;
}
void glDeleteTransformFeedbacks ( GLsizei n_, const GLuint * ids_ ){
    static PFNGLDELETETRANSFORMFEEDBACKSPROC F;
    if(!F)
        F = ( PFNGLDELETETRANSFORMFEEDBACKSPROC ) mygetprocaddr("glDeleteTransformFeedbacks");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) ids_ );
        for(auto& it : wildcardHooks )
            it("glDeleteTransformFeedbacks",params);
    }
    F(n_, ids_);
    return;
}
void glDeleteVertexArrays ( GLsizei n_, const GLuint * arrays_ ){
    static PFNGLDELETEVERTEXARRAYSPROC F;
    if(!F)
        F = ( PFNGLDELETEVERTEXARRAYSPROC ) mygetprocaddr("glDeleteVertexArrays");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) arrays_ );
        for(auto& it : wildcardHooks )
            it("glDeleteVertexArrays",params);
    }
    F(n_, arrays_);
    return;
}
void glDepthFunc ( GLenum func_ ){
    static PFNGLDEPTHFUNCPROC F;
    if(!F)
        F = ( PFNGLDEPTHFUNCPROC ) mygetprocaddr("glDepthFunc");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) func_ );
        for(auto& it : wildcardHooks )
            it("glDepthFunc",params);
    }
    F(func_);
    return;
}
void glDepthMask ( GLboolean flag_ ){
    static PFNGLDEPTHMASKPROC F;
    if(!F)
        F = ( PFNGLDEPTHMASKPROC ) mygetprocaddr("glDepthMask");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) flag_ );
        for(auto& it : wildcardHooks )
            it("glDepthMask",params);
    }
    F(flag_);
    return;
}
void glDepthRange ( GLdouble near_, GLdouble far_ ){
    static PFNGLDEPTHRANGEPROC F;
    if(!F)
        F = ( PFNGLDEPTHRANGEPROC ) mygetprocaddr("glDepthRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) near_ );
        params.push_back( (uint64_t) far_ );
        for(auto& it : wildcardHooks )
            it("glDepthRange",params);
    }
    F(near_, far_);
    return;
}
void glDepthRangeArrayv ( GLuint first_, GLsizei count_, const GLdouble * v_ ){
    static PFNGLDEPTHRANGEARRAYVPROC F;
    if(!F)
        F = ( PFNGLDEPTHRANGEARRAYVPROC ) mygetprocaddr("glDepthRangeArrayv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glDepthRangeArrayv",params);
    }
    F(first_, count_, v_);
    return;
}
void glDepthRangeIndexed ( GLuint index_, GLdouble n_, GLdouble f_ ){
    static PFNGLDEPTHRANGEINDEXEDPROC F;
    if(!F)
        F = ( PFNGLDEPTHRANGEINDEXEDPROC ) mygetprocaddr("glDepthRangeIndexed");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) f_ );
        for(auto& it : wildcardHooks )
            it("glDepthRangeIndexed",params);
    }
    F(index_, n_, f_);
    return;
}
void glDepthRangef ( GLfloat n_, GLfloat f_ ){
    static PFNGLDEPTHRANGEFPROC F;
    if(!F)
        F = ( PFNGLDEPTHRANGEFPROC ) mygetprocaddr("glDepthRangef");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) f_ );
        for(auto& it : wildcardHooks )
            it("glDepthRangef",params);
    }
    F(n_, f_);
    return;
}
void glDetachShader ( GLuint program_, GLuint shader_ ){
    static PFNGLDETACHSHADERPROC F;
    if(!F)
        F = ( PFNGLDETACHSHADERPROC ) mygetprocaddr("glDetachShader");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shader_ );
        for(auto& it : wildcardHooks )
            it("glDetachShader",params);
    }
    F(program_, shader_);
    return;
}
void glDisable ( GLenum cap_ ){
    static PFNGLDISABLEPROC F;
    if(!F)
        F = ( PFNGLDISABLEPROC ) mygetprocaddr("glDisable");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) cap_ );
        for(auto& it : wildcardHooks )
            it("glDisable",params);
    }
    F(cap_);
    return;
}
void glDisableVertexArrayAttrib ( GLuint vaobj_, GLuint index_ ){
    static PFNGLDISABLEVERTEXARRAYATTRIBPROC F;
    if(!F)
        F = ( PFNGLDISABLEVERTEXARRAYATTRIBPROC ) mygetprocaddr("glDisableVertexArrayAttrib");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glDisableVertexArrayAttrib",params);
    }
    F(vaobj_, index_);
    return;
}
void glDisableVertexAttribArray ( GLuint index_ ){
    static PFNGLDISABLEVERTEXATTRIBARRAYPROC F;
    if(!F)
        F = ( PFNGLDISABLEVERTEXATTRIBARRAYPROC ) mygetprocaddr("glDisableVertexAttribArray");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glDisableVertexAttribArray",params);
    }
    F(index_);
    return;
}
void glDisablei ( GLenum target_, GLuint index_ ){
    static PFNGLDISABLEIPROC F;
    if(!F)
        F = ( PFNGLDISABLEIPROC ) mygetprocaddr("glDisablei");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glDisablei",params);
    }
    F(target_, index_);
    return;
}
void glDispatchCompute ( GLuint num_groups_x_, GLuint num_groups_y_, GLuint num_groups_z_ ){
    static PFNGLDISPATCHCOMPUTEPROC F;
    if(!F)
        F = ( PFNGLDISPATCHCOMPUTEPROC ) mygetprocaddr("glDispatchCompute");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) num_groups_x_ );
        params.push_back( (uint64_t) num_groups_y_ );
        params.push_back( (uint64_t) num_groups_z_ );
        for(auto& it : drawCallHooks )
            it("glDispatchCompute",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) num_groups_x_ );
        params.push_back( (uint64_t) num_groups_y_ );
        params.push_back( (uint64_t) num_groups_z_ );
        for(auto& it : wildcardHooks )
            it("glDispatchCompute",params);
    }
    F(num_groups_x_, num_groups_y_, num_groups_z_);
    return;
}
void glDispatchComputeIndirect ( GLintptr indirect_ ){
    static PFNGLDISPATCHCOMPUTEINDIRECTPROC F;
    if(!F)
        F = ( PFNGLDISPATCHCOMPUTEINDIRECTPROC ) mygetprocaddr("glDispatchComputeIndirect");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) indirect_ );
        for(auto& it : drawCallHooks )
            it("glDispatchComputeIndirect",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) indirect_ );
        for(auto& it : wildcardHooks )
            it("glDispatchComputeIndirect",params);
    }
    F(indirect_);
    return;
}
void glDrawArrays ( GLenum mode_, GLint first_, GLsizei count_ ){
    static PFNGLDRAWARRAYSPROC F;
    if(!F)
        F = ( PFNGLDRAWARRAYSPROC ) mygetprocaddr("glDrawArrays");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        for(auto& it : drawCallHooks )
            it("glDrawArrays",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        for(auto& it : wildcardHooks )
            it("glDrawArrays",params);
    }
    F(mode_, first_, count_);
    return;
}
void glDrawArraysIndirect ( GLenum mode_, const void * indirect_ ){
    static PFNGLDRAWARRAYSINDIRECTPROC F;
    if(!F)
        F = ( PFNGLDRAWARRAYSINDIRECTPROC ) mygetprocaddr("glDrawArraysIndirect");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) indirect_ );
        for(auto& it : drawCallHooks )
            it("glDrawArraysIndirect",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) indirect_ );
        for(auto& it : wildcardHooks )
            it("glDrawArraysIndirect",params);
    }
    F(mode_, indirect_);
    return;
}
void glDrawArraysInstanced ( GLenum mode_, GLint first_, GLsizei count_, GLsizei instancecount_ ){
    static PFNGLDRAWARRAYSINSTANCEDPROC F;
    if(!F)
        F = ( PFNGLDRAWARRAYSINSTANCEDPROC ) mygetprocaddr("glDrawArraysInstanced");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) instancecount_ );
        for(auto& it : drawCallHooks )
            it("glDrawArraysInstanced",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) instancecount_ );
        for(auto& it : wildcardHooks )
            it("glDrawArraysInstanced",params);
    }
    F(mode_, first_, count_, instancecount_);
    return;
}
void glDrawArraysInstancedBaseInstance ( GLenum mode_, GLint first_, GLsizei count_, GLsizei instancecount_, GLuint baseinstance_ ){
    static PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC F;
    if(!F)
        F = ( PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC ) mygetprocaddr("glDrawArraysInstancedBaseInstance");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) baseinstance_ );
        for(auto& it : drawCallHooks )
            it("glDrawArraysInstancedBaseInstance",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) baseinstance_ );
        for(auto& it : wildcardHooks )
            it("glDrawArraysInstancedBaseInstance",params);
    }
    F(mode_, first_, count_, instancecount_, baseinstance_);
    return;
}
void glDrawBuffer ( GLenum buf_ ){
    static PFNGLDRAWBUFFERPROC F;
    if(!F)
        F = ( PFNGLDRAWBUFFERPROC ) mygetprocaddr("glDrawBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buf_ );
        for(auto& it : wildcardHooks )
            it("glDrawBuffer",params);
    }
    F(buf_);
    return;
}
void glDrawBuffers ( GLsizei n_, const GLenum * bufs_ ){
    static PFNGLDRAWBUFFERSPROC F;
    if(!F)
        F = ( PFNGLDRAWBUFFERSPROC ) mygetprocaddr("glDrawBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) bufs_ );
        for(auto& it : wildcardHooks )
            it("glDrawBuffers",params);
    }
    F(n_, bufs_);
    return;
}
void glDrawElements ( GLenum mode_, GLsizei count_, GLenum type_, const void * indices_ ){
    static PFNGLDRAWELEMENTSPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSPROC ) mygetprocaddr("glDrawElements");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        for(auto& it : drawCallHooks )
            it("glDrawElements",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        for(auto& it : wildcardHooks )
            it("glDrawElements",params);
    }
    F(mode_, count_, type_, indices_);
    return;
}
void glDrawElementsBaseVertex ( GLenum mode_, GLsizei count_, GLenum type_, const void * indices_, GLint basevertex_ ){
    static PFNGLDRAWELEMENTSBASEVERTEXPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSBASEVERTEXPROC ) mygetprocaddr("glDrawElementsBaseVertex");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : drawCallHooks )
            it("glDrawElementsBaseVertex",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : wildcardHooks )
            it("glDrawElementsBaseVertex",params);
    }
    F(mode_, count_, type_, indices_, basevertex_);
    return;
}
void glDrawElementsIndirect ( GLenum mode_, GLenum type_, const void * indirect_ ){
    static PFNGLDRAWELEMENTSINDIRECTPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSINDIRECTPROC ) mygetprocaddr("glDrawElementsIndirect");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indirect_ );
        for(auto& it : drawCallHooks )
            it("glDrawElementsIndirect",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indirect_ );
        for(auto& it : wildcardHooks )
            it("glDrawElementsIndirect",params);
    }
    F(mode_, type_, indirect_);
    return;
}
void glDrawElementsInstanced ( GLenum mode_, GLsizei count_, GLenum type_, const void * indices_, GLsizei instancecount_ ){
    static PFNGLDRAWELEMENTSINSTANCEDPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSINSTANCEDPROC ) mygetprocaddr("glDrawElementsInstanced");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        for(auto& it : drawCallHooks )
            it("glDrawElementsInstanced",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        for(auto& it : wildcardHooks )
            it("glDrawElementsInstanced",params);
    }
    F(mode_, count_, type_, indices_, instancecount_);
    return;
}
void glDrawElementsInstancedBaseInstance ( GLenum mode_, GLsizei count_, GLenum type_, const void * indices_, GLsizei instancecount_, GLuint baseinstance_ ){
    static PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC ) mygetprocaddr("glDrawElementsInstancedBaseInstance");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) baseinstance_ );
        for(auto& it : drawCallHooks )
            it("glDrawElementsInstancedBaseInstance",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) baseinstance_ );
        for(auto& it : wildcardHooks )
            it("glDrawElementsInstancedBaseInstance",params);
    }
    F(mode_, count_, type_, indices_, instancecount_, baseinstance_);
    return;
}
void glDrawElementsInstancedBaseVertex ( GLenum mode_, GLsizei count_, GLenum type_, const void * indices_, GLsizei instancecount_, GLint basevertex_ ){
    static PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC ) mygetprocaddr("glDrawElementsInstancedBaseVertex");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : drawCallHooks )
            it("glDrawElementsInstancedBaseVertex",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : wildcardHooks )
            it("glDrawElementsInstancedBaseVertex",params);
    }
    F(mode_, count_, type_, indices_, instancecount_, basevertex_);
    return;
}
void glDrawElementsInstancedBaseVertexBaseInstance ( GLenum mode_, GLsizei count_, GLenum type_, const void * indices_, GLsizei instancecount_, GLint basevertex_, GLuint baseinstance_ ){
    static PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC F;
    if(!F)
        F = ( PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC ) mygetprocaddr("glDrawElementsInstancedBaseVertexBaseInstance");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) basevertex_ );
        params.push_back( (uint64_t) baseinstance_ );
        for(auto& it : drawCallHooks )
            it("glDrawElementsInstancedBaseVertexBaseInstance",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) instancecount_ );
        params.push_back( (uint64_t) basevertex_ );
        params.push_back( (uint64_t) baseinstance_ );
        for(auto& it : wildcardHooks )
            it("glDrawElementsInstancedBaseVertexBaseInstance",params);
    }
    F(mode_, count_, type_, indices_, instancecount_, basevertex_, baseinstance_);
    return;
}
void glDrawRangeElements ( GLenum mode_, GLuint start_, GLuint end_, GLsizei count_, GLenum type_, const void * indices_ ){
    static PFNGLDRAWRANGEELEMENTSPROC F;
    if(!F)
        F = ( PFNGLDRAWRANGEELEMENTSPROC ) mygetprocaddr("glDrawRangeElements");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) start_ );
        params.push_back( (uint64_t) end_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        for(auto& it : drawCallHooks )
            it("glDrawRangeElements",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) start_ );
        params.push_back( (uint64_t) end_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        for(auto& it : wildcardHooks )
            it("glDrawRangeElements",params);
    }
    F(mode_, start_, end_, count_, type_, indices_);
    return;
}
void glDrawRangeElementsBaseVertex ( GLenum mode_, GLuint start_, GLuint end_, GLsizei count_, GLenum type_, const void * indices_, GLint basevertex_ ){
    static PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC F;
    if(!F)
        F = ( PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC ) mygetprocaddr("glDrawRangeElementsBaseVertex");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) start_ );
        params.push_back( (uint64_t) end_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : drawCallHooks )
            it("glDrawRangeElementsBaseVertex",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) start_ );
        params.push_back( (uint64_t) end_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : wildcardHooks )
            it("glDrawRangeElementsBaseVertex",params);
    }
    F(mode_, start_, end_, count_, type_, indices_, basevertex_);
    return;
}
void glDrawTransformFeedback ( GLenum mode_, GLuint id_ ){
    static PFNGLDRAWTRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLDRAWTRANSFORMFEEDBACKPROC ) mygetprocaddr("glDrawTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) id_ );
        for(auto& it : wildcardHooks )
            it("glDrawTransformFeedback",params);
    }
    F(mode_, id_);
    return;
}
void glDrawTransformFeedbackInstanced ( GLenum mode_, GLuint id_, GLsizei instancecount_ ){
    static PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC F;
    if(!F)
        F = ( PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC ) mygetprocaddr("glDrawTransformFeedbackInstanced");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) instancecount_ );
        for(auto& it : wildcardHooks )
            it("glDrawTransformFeedbackInstanced",params);
    }
    F(mode_, id_, instancecount_);
    return;
}
void glDrawTransformFeedbackStream ( GLenum mode_, GLuint id_, GLuint stream_ ){
    static PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC F;
    if(!F)
        F = ( PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC ) mygetprocaddr("glDrawTransformFeedbackStream");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) stream_ );
        for(auto& it : wildcardHooks )
            it("glDrawTransformFeedbackStream",params);
    }
    F(mode_, id_, stream_);
    return;
}
void glDrawTransformFeedbackStreamInstanced ( GLenum mode_, GLuint id_, GLuint stream_, GLsizei instancecount_ ){
    static PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC F;
    if(!F)
        F = ( PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC ) mygetprocaddr("glDrawTransformFeedbackStreamInstanced");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) stream_ );
        params.push_back( (uint64_t) instancecount_ );
        for(auto& it : wildcardHooks )
            it("glDrawTransformFeedbackStreamInstanced",params);
    }
    F(mode_, id_, stream_, instancecount_);
    return;
}
void glEnable ( GLenum cap_ ){
    static PFNGLENABLEPROC F;
    if(!F)
        F = ( PFNGLENABLEPROC ) mygetprocaddr("glEnable");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) cap_ );
        for(auto& it : wildcardHooks )
            it("glEnable",params);
    }
    F(cap_);
    return;
}
void glEnableVertexArrayAttrib ( GLuint vaobj_, GLuint index_ ){
    static PFNGLENABLEVERTEXARRAYATTRIBPROC F;
    if(!F)
        F = ( PFNGLENABLEVERTEXARRAYATTRIBPROC ) mygetprocaddr("glEnableVertexArrayAttrib");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glEnableVertexArrayAttrib",params);
    }
    F(vaobj_, index_);
    return;
}
void glEnableVertexAttribArray ( GLuint index_ ){
    static PFNGLENABLEVERTEXATTRIBARRAYPROC F;
    if(!F)
        F = ( PFNGLENABLEVERTEXATTRIBARRAYPROC ) mygetprocaddr("glEnableVertexAttribArray");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glEnableVertexAttribArray",params);
    }
    F(index_);
    return;
}
void glEnablei ( GLenum target_, GLuint index_ ){
    static PFNGLENABLEIPROC F;
    if(!F)
        F = ( PFNGLENABLEIPROC ) mygetprocaddr("glEnablei");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glEnablei",params);
    }
    F(target_, index_);
    return;
}
void glEndConditionalRender (  ){
    static PFNGLENDCONDITIONALRENDERPROC F;
    if(!F)
        F = ( PFNGLENDCONDITIONALRENDERPROC ) mygetprocaddr("glEndConditionalRender");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glEndConditionalRender",params);
    }
    F();
    return;
}
void glEndQuery ( GLenum target_ ){
    static PFNGLENDQUERYPROC F;
    if(!F)
        F = ( PFNGLENDQUERYPROC ) mygetprocaddr("glEndQuery");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        for(auto& it : wildcardHooks )
            it("glEndQuery",params);
    }
    F(target_);
    return;
}
void glEndQueryIndexed ( GLenum target_, GLuint index_ ){
    static PFNGLENDQUERYINDEXEDPROC F;
    if(!F)
        F = ( PFNGLENDQUERYINDEXEDPROC ) mygetprocaddr("glEndQueryIndexed");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glEndQueryIndexed",params);
    }
    F(target_, index_);
    return;
}
void glEndTransformFeedback (  ){
    static PFNGLENDTRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLENDTRANSFORMFEEDBACKPROC ) mygetprocaddr("glEndTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glEndTransformFeedback",params);
    }
    F();
    return;
}
GLsync glFenceSync ( GLenum condition_, GLbitfield flags_ ){
    static PFNGLFENCESYNCPROC F;
    if(!F)
        F = ( PFNGLFENCESYNCPROC ) mygetprocaddr("glFenceSync");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) condition_ );
        params.push_back( (uint64_t) flags_ );
        for(auto& it : wildcardHooks )
            it("glFenceSync",params);
    }
    GLsync retval = F(condition_, flags_);
    return retval;
}
void glFinish (  ){
    static PFNGLFINISHPROC F;
    if(!F)
        F = ( PFNGLFINISHPROC ) mygetprocaddr("glFinish");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glFinish",params);
    }
    F();
    return;
}
void glFlush (  ){
    static PFNGLFLUSHPROC F;
    if(!F)
        F = ( PFNGLFLUSHPROC ) mygetprocaddr("glFlush");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glFlush",params);
    }
    F();
    return;
}
void glFlushMappedBufferRange ( GLenum target_, GLintptr offset_, GLsizeiptr length_ ){
    static PFNGLFLUSHMAPPEDBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLFLUSHMAPPEDBUFFERRANGEPROC ) mygetprocaddr("glFlushMappedBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) length_ );
        for(auto& it : wildcardHooks )
            it("glFlushMappedBufferRange",params);
    }
    F(target_, offset_, length_);
    return;
}
void glFlushMappedNamedBufferRange ( GLuint buffer_, GLintptr offset_, GLsizeiptr length_ ){
    static PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC ) mygetprocaddr("glFlushMappedNamedBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) length_ );
        for(auto& it : wildcardHooks )
            it("glFlushMappedNamedBufferRange",params);
    }
    F(buffer_, offset_, length_);
    return;
}
void glFramebufferParameteri ( GLenum target_, GLenum pname_, GLint param_ ){
    static PFNGLFRAMEBUFFERPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERPARAMETERIPROC ) mygetprocaddr("glFramebufferParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferParameteri",params);
    }
    F(target_, pname_, param_);
    return;
}
void glFramebufferRenderbuffer ( GLenum target_, GLenum attachment_, GLenum renderbuffertarget_, GLuint renderbuffer_ ){
    static PFNGLFRAMEBUFFERRENDERBUFFERPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERRENDERBUFFERPROC ) mygetprocaddr("glFramebufferRenderbuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) renderbuffertarget_ );
        params.push_back( (uint64_t) renderbuffer_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferRenderbuffer",params);
    }
    F(target_, attachment_, renderbuffertarget_, renderbuffer_);
    return;
}
void glFramebufferTexture ( GLenum target_, GLenum attachment_, GLuint texture_, GLint level_ ){
    static PFNGLFRAMEBUFFERTEXTUREPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERTEXTUREPROC ) mygetprocaddr("glFramebufferTexture");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferTexture",params);
    }
    F(target_, attachment_, texture_, level_);
    return;
}
void glFramebufferTexture1D ( GLenum target_, GLenum attachment_, GLenum textarget_, GLuint texture_, GLint level_ ){
    static PFNGLFRAMEBUFFERTEXTURE1DPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERTEXTURE1DPROC ) mygetprocaddr("glFramebufferTexture1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) textarget_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferTexture1D",params);
    }
    F(target_, attachment_, textarget_, texture_, level_);
    return;
}
void glFramebufferTexture2D ( GLenum target_, GLenum attachment_, GLenum textarget_, GLuint texture_, GLint level_ ){
    static PFNGLFRAMEBUFFERTEXTURE2DPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERTEXTURE2DPROC ) mygetprocaddr("glFramebufferTexture2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) textarget_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferTexture2D",params);
    }
    F(target_, attachment_, textarget_, texture_, level_);
    return;
}
void glFramebufferTexture3D ( GLenum target_, GLenum attachment_, GLenum textarget_, GLuint texture_, GLint level_, GLint zoffset_ ){
    static PFNGLFRAMEBUFFERTEXTURE3DPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERTEXTURE3DPROC ) mygetprocaddr("glFramebufferTexture3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) textarget_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) zoffset_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferTexture3D",params);
    }
    F(target_, attachment_, textarget_, texture_, level_, zoffset_);
    return;
}
void glFramebufferTextureLayer ( GLenum target_, GLenum attachment_, GLuint texture_, GLint level_, GLint layer_ ){
    static PFNGLFRAMEBUFFERTEXTURELAYERPROC F;
    if(!F)
        F = ( PFNGLFRAMEBUFFERTEXTURELAYERPROC ) mygetprocaddr("glFramebufferTextureLayer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) layer_ );
        for(auto& it : wildcardHooks )
            it("glFramebufferTextureLayer",params);
    }
    F(target_, attachment_, texture_, level_, layer_);
    return;
}
void glFrontFace ( GLenum mode_ ){
    static PFNGLFRONTFACEPROC F;
    if(!F)
        F = ( PFNGLFRONTFACEPROC ) mygetprocaddr("glFrontFace");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glFrontFace",params);
    }
    F(mode_);
    return;
}
void glGenBuffers ( GLsizei n_, GLuint * buffers_ ){
    static PFNGLGENBUFFERSPROC F;
    if(!F)
        F = ( PFNGLGENBUFFERSPROC ) mygetprocaddr("glGenBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) buffers_ );
        for(auto& it : wildcardHooks )
            it("glGenBuffers",params);
    }
    F(n_, buffers_);
    return;
}
void glGenFramebuffers ( GLsizei n_, GLuint * framebuffers_ ){
    static PFNGLGENFRAMEBUFFERSPROC F;
    if(!F)
        F = ( PFNGLGENFRAMEBUFFERSPROC ) mygetprocaddr("glGenFramebuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) framebuffers_ );
        for(auto& it : wildcardHooks )
            it("glGenFramebuffers",params);
    }
    F(n_, framebuffers_);
    return;
}
void glGenProgramPipelines ( GLsizei n_, GLuint * pipelines_ ){
    static PFNGLGENPROGRAMPIPELINESPROC F;
    if(!F)
        F = ( PFNGLGENPROGRAMPIPELINESPROC ) mygetprocaddr("glGenProgramPipelines");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) pipelines_ );
        for(auto& it : wildcardHooks )
            it("glGenProgramPipelines",params);
    }
    F(n_, pipelines_);
    return;
}
void glGenQueries ( GLsizei n_, GLuint * ids_ ){
    static PFNGLGENQUERIESPROC F;
    if(!F)
        F = ( PFNGLGENQUERIESPROC ) mygetprocaddr("glGenQueries");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) ids_ );
        for(auto& it : wildcardHooks )
            it("glGenQueries",params);
    }
    F(n_, ids_);
    return;
}
void glGenRenderbuffers ( GLsizei n_, GLuint * renderbuffers_ ){
    static PFNGLGENRENDERBUFFERSPROC F;
    if(!F)
        F = ( PFNGLGENRENDERBUFFERSPROC ) mygetprocaddr("glGenRenderbuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) renderbuffers_ );
        for(auto& it : wildcardHooks )
            it("glGenRenderbuffers",params);
    }
    F(n_, renderbuffers_);
    return;
}
void glGenSamplers ( GLsizei count_, GLuint * samplers_ ){
    static PFNGLGENSAMPLERSPROC F;
    if(!F)
        F = ( PFNGLGENSAMPLERSPROC ) mygetprocaddr("glGenSamplers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) samplers_ );
        for(auto& it : wildcardHooks )
            it("glGenSamplers",params);
    }
    F(count_, samplers_);
    return;
}
void glGenTextures ( GLsizei n_, GLuint * textures_ ){
    static PFNGLGENTEXTURESPROC F;
    if(!F)
        F = ( PFNGLGENTEXTURESPROC ) mygetprocaddr("glGenTextures");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) textures_ );
        for(auto& it : wildcardHooks )
            it("glGenTextures",params);
    }
    F(n_, textures_);
    return;
}
void glGenTransformFeedbacks ( GLsizei n_, GLuint * ids_ ){
    static PFNGLGENTRANSFORMFEEDBACKSPROC F;
    if(!F)
        F = ( PFNGLGENTRANSFORMFEEDBACKSPROC ) mygetprocaddr("glGenTransformFeedbacks");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) ids_ );
        for(auto& it : wildcardHooks )
            it("glGenTransformFeedbacks",params);
    }
    F(n_, ids_);
    return;
}
void glGenVertexArrays ( GLsizei n_, GLuint * arrays_ ){
    static PFNGLGENVERTEXARRAYSPROC F;
    if(!F)
        F = ( PFNGLGENVERTEXARRAYSPROC ) mygetprocaddr("glGenVertexArrays");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) arrays_ );
        for(auto& it : wildcardHooks )
            it("glGenVertexArrays",params);
    }
    F(n_, arrays_);
    return;
}
void glGenerateMipmap ( GLenum target_ ){
    static PFNGLGENERATEMIPMAPPROC F;
    if(!F)
        F = ( PFNGLGENERATEMIPMAPPROC ) mygetprocaddr("glGenerateMipmap");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        for(auto& it : wildcardHooks )
            it("glGenerateMipmap",params);
    }
    F(target_);
    return;
}
void glGenerateTextureMipmap ( GLuint texture_ ){
    static PFNGLGENERATETEXTUREMIPMAPPROC F;
    if(!F)
        F = ( PFNGLGENERATETEXTUREMIPMAPPROC ) mygetprocaddr("glGenerateTextureMipmap");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        for(auto& it : wildcardHooks )
            it("glGenerateTextureMipmap",params);
    }
    F(texture_);
    return;
}
void glGetActiveAtomicCounterBufferiv ( GLuint program_, GLuint bufferIndex_, GLenum pname_, GLint * params_ ){
    static PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC ) mygetprocaddr("glGetActiveAtomicCounterBufferiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) bufferIndex_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveAtomicCounterBufferiv",params);
    }
    F(program_, bufferIndex_, pname_, params_);
    return;
}
void glGetActiveAttrib ( GLuint program_, GLuint index_, GLsizei bufSize_, GLsizei * length_, GLint * size_, GLenum * type_, GLchar * name_ ){
    static PFNGLGETACTIVEATTRIBPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEATTRIBPROC ) mygetprocaddr("glGetActiveAttrib");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveAttrib",params);
    }
    F(program_, index_, bufSize_, length_, size_, type_, name_);
    return;
}
void glGetActiveSubroutineName ( GLuint program_, GLenum shadertype_, GLuint index_, GLsizei bufsize_, GLsizei * length_, GLchar * name_ ){
    static PFNGLGETACTIVESUBROUTINENAMEPROC F;
    if(!F)
        F = ( PFNGLGETACTIVESUBROUTINENAMEPROC ) mygetprocaddr("glGetActiveSubroutineName");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) bufsize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveSubroutineName",params);
    }
    F(program_, shadertype_, index_, bufsize_, length_, name_);
    return;
}
void glGetActiveSubroutineUniformName ( GLuint program_, GLenum shadertype_, GLuint index_, GLsizei bufsize_, GLsizei * length_, GLchar * name_ ){
    static PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC F;
    if(!F)
        F = ( PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC ) mygetprocaddr("glGetActiveSubroutineUniformName");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) bufsize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveSubroutineUniformName",params);
    }
    F(program_, shadertype_, index_, bufsize_, length_, name_);
    return;
}
void glGetActiveSubroutineUniformiv ( GLuint program_, GLenum shadertype_, GLuint index_, GLenum pname_, GLint * values_ ){
    static PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC F;
    if(!F)
        F = ( PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC ) mygetprocaddr("glGetActiveSubroutineUniformiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) values_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveSubroutineUniformiv",params);
    }
    F(program_, shadertype_, index_, pname_, values_);
    return;
}
void glGetActiveUniform ( GLuint program_, GLuint index_, GLsizei bufSize_, GLsizei * length_, GLint * size_, GLenum * type_, GLchar * name_ ){
    static PFNGLGETACTIVEUNIFORMPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEUNIFORMPROC ) mygetprocaddr("glGetActiveUniform");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveUniform",params);
    }
    F(program_, index_, bufSize_, length_, size_, type_, name_);
    return;
}
void glGetActiveUniformBlockName ( GLuint program_, GLuint uniformBlockIndex_, GLsizei bufSize_, GLsizei * length_, GLchar * uniformBlockName_ ){
    static PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC ) mygetprocaddr("glGetActiveUniformBlockName");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformBlockIndex_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) uniformBlockName_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveUniformBlockName",params);
    }
    F(program_, uniformBlockIndex_, bufSize_, length_, uniformBlockName_);
    return;
}
void glGetActiveUniformBlockiv ( GLuint program_, GLuint uniformBlockIndex_, GLenum pname_, GLint * params_ ){
    static PFNGLGETACTIVEUNIFORMBLOCKIVPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEUNIFORMBLOCKIVPROC ) mygetprocaddr("glGetActiveUniformBlockiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformBlockIndex_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveUniformBlockiv",params);
    }
    F(program_, uniformBlockIndex_, pname_, params_);
    return;
}
void glGetActiveUniformName ( GLuint program_, GLuint uniformIndex_, GLsizei bufSize_, GLsizei * length_, GLchar * uniformName_ ){
    static PFNGLGETACTIVEUNIFORMNAMEPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEUNIFORMNAMEPROC ) mygetprocaddr("glGetActiveUniformName");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformIndex_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) uniformName_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveUniformName",params);
    }
    F(program_, uniformIndex_, bufSize_, length_, uniformName_);
    return;
}
void glGetActiveUniformsiv ( GLuint program_, GLsizei uniformCount_, const GLuint * uniformIndices_, GLenum pname_, GLint * params_ ){
    static PFNGLGETACTIVEUNIFORMSIVPROC F;
    if(!F)
        F = ( PFNGLGETACTIVEUNIFORMSIVPROC ) mygetprocaddr("glGetActiveUniformsiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformCount_ );
        params.push_back( (uint64_t) uniformIndices_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetActiveUniformsiv",params);
    }
    F(program_, uniformCount_, uniformIndices_, pname_, params_);
    return;
}
void glGetAttachedShaders ( GLuint program_, GLsizei maxCount_, GLsizei * count_, GLuint * shaders_ ){
    static PFNGLGETATTACHEDSHADERSPROC F;
    if(!F)
        F = ( PFNGLGETATTACHEDSHADERSPROC ) mygetprocaddr("glGetAttachedShaders");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) maxCount_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) shaders_ );
        for(auto& it : wildcardHooks )
            it("glGetAttachedShaders",params);
    }
    F(program_, maxCount_, count_, shaders_);
    return;
}
GLint glGetAttribLocation ( GLuint program_, const GLchar * name_ ){
    static PFNGLGETATTRIBLOCATIONPROC F;
    if(!F)
        F = ( PFNGLGETATTRIBLOCATIONPROC ) mygetprocaddr("glGetAttribLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetAttribLocation",params);
    }
    GLint retval = F(program_, name_);
    return retval;
}
void glGetBooleani_v ( GLenum target_, GLuint index_, GLboolean * data_ ){
    static PFNGLGETBOOLEANI_VPROC F;
    if(!F)
        F = ( PFNGLGETBOOLEANI_VPROC ) mygetprocaddr("glGetBooleani_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetBooleani_v",params);
    }
    F(target_, index_, data_);
    return;
}
void glGetBooleanv ( GLenum pname_, GLboolean * data_ ){
    static PFNGLGETBOOLEANVPROC F;
    if(!F)
        F = ( PFNGLGETBOOLEANVPROC ) mygetprocaddr("glGetBooleanv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetBooleanv",params);
    }
    F(pname_, data_);
    return;
}
void glGetBufferParameteri64v ( GLenum target_, GLenum pname_, GLint64 * params_ ){
    static PFNGLGETBUFFERPARAMETERI64VPROC F;
    if(!F)
        F = ( PFNGLGETBUFFERPARAMETERI64VPROC ) mygetprocaddr("glGetBufferParameteri64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetBufferParameteri64v",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetBufferParameteriv ( GLenum target_, GLenum pname_, GLint * params_ ){
    static PFNGLGETBUFFERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETBUFFERPARAMETERIVPROC ) mygetprocaddr("glGetBufferParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetBufferParameteriv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetBufferPointerv ( GLenum target_, GLenum pname_, void ** params_ ){
    static PFNGLGETBUFFERPOINTERVPROC F;
    if(!F)
        F = ( PFNGLGETBUFFERPOINTERVPROC ) mygetprocaddr("glGetBufferPointerv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetBufferPointerv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetBufferSubData ( GLenum target_, GLintptr offset_, GLsizeiptr size_, void * data_ ){
    static PFNGLGETBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLGETBUFFERSUBDATAPROC ) mygetprocaddr("glGetBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetBufferSubData",params);
    }
    F(target_, offset_, size_, data_);
    return;
}
void glGetCompressedTexImage ( GLenum target_, GLint level_, void * img_ ){
    static PFNGLGETCOMPRESSEDTEXIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETCOMPRESSEDTEXIMAGEPROC ) mygetprocaddr("glGetCompressedTexImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) img_ );
        for(auto& it : wildcardHooks )
            it("glGetCompressedTexImage",params);
    }
    F(target_, level_, img_);
    return;
}
void glGetCompressedTextureImage ( GLuint texture_, GLint level_, GLsizei bufSize_, void * pixels_ ){
    static PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC ) mygetprocaddr("glGetCompressedTextureImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetCompressedTextureImage",params);
    }
    F(texture_, level_, bufSize_, pixels_);
    return;
}
void glGetCompressedTextureSubImage ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLsizei bufSize_, void * pixels_ ){
    static PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC ) mygetprocaddr("glGetCompressedTextureSubImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetCompressedTextureSubImage",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, bufSize_, pixels_);
    return;
}
GLuint glGetDebugMessageLog ( GLuint count_, GLsizei bufSize_, GLenum * sources_, GLenum * types_, GLuint * ids_, GLenum * severities_, GLsizei * lengths_, GLchar * messageLog_ ){
    static PFNGLGETDEBUGMESSAGELOGPROC F;
    if(!F)
        F = ( PFNGLGETDEBUGMESSAGELOGPROC ) mygetprocaddr("glGetDebugMessageLog");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) sources_ );
        params.push_back( (uint64_t) types_ );
        params.push_back( (uint64_t) ids_ );
        params.push_back( (uint64_t) severities_ );
        params.push_back( (uint64_t) lengths_ );
        params.push_back( (uint64_t) messageLog_ );
        for(auto& it : wildcardHooks )
            it("glGetDebugMessageLog",params);
    }
    GLuint retval = F(count_, bufSize_, sources_, types_, ids_, severities_, lengths_, messageLog_);
    return retval;
}
void glGetDoublei_v ( GLenum target_, GLuint index_, GLdouble * data_ ){
    static PFNGLGETDOUBLEI_VPROC F;
    if(!F)
        F = ( PFNGLGETDOUBLEI_VPROC ) mygetprocaddr("glGetDoublei_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetDoublei_v",params);
    }
    F(target_, index_, data_);
    return;
}
void glGetDoublev ( GLenum pname_, GLdouble * data_ ){
    static PFNGLGETDOUBLEVPROC F;
    if(!F)
        F = ( PFNGLGETDOUBLEVPROC ) mygetprocaddr("glGetDoublev");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetDoublev",params);
    }
    F(pname_, data_);
    return;
}
GLenum glGetError (  ){
    static PFNGLGETERRORPROC F;
    if(!F)
        F = ( PFNGLGETERRORPROC ) mygetprocaddr("glGetError");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glGetError",params);
    }
    GLenum retval = F();
    return retval;
}
void glGetFloati_v ( GLenum target_, GLuint index_, GLfloat * data_ ){
    static PFNGLGETFLOATI_VPROC F;
    if(!F)
        F = ( PFNGLGETFLOATI_VPROC ) mygetprocaddr("glGetFloati_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetFloati_v",params);
    }
    F(target_, index_, data_);
    return;
}
void glGetFloatv ( GLenum pname_, GLfloat * data_ ){
    static PFNGLGETFLOATVPROC F;
    if(!F)
        F = ( PFNGLGETFLOATVPROC ) mygetprocaddr("glGetFloatv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetFloatv",params);
    }
    F(pname_, data_);
    return;
}
GLint glGetFragDataIndex ( GLuint program_, const GLchar * name_ ){
    static PFNGLGETFRAGDATAINDEXPROC F;
    if(!F)
        F = ( PFNGLGETFRAGDATAINDEXPROC ) mygetprocaddr("glGetFragDataIndex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetFragDataIndex",params);
    }
    GLint retval = F(program_, name_);
    return retval;
}
GLint glGetFragDataLocation ( GLuint program_, const GLchar * name_ ){
    static PFNGLGETFRAGDATALOCATIONPROC F;
    if(!F)
        F = ( PFNGLGETFRAGDATALOCATIONPROC ) mygetprocaddr("glGetFragDataLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetFragDataLocation",params);
    }
    GLint retval = F(program_, name_);
    return retval;
}
void glGetFramebufferAttachmentParameteriv ( GLenum target_, GLenum attachment_, GLenum pname_, GLint * params_ ){
    static PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC ) mygetprocaddr("glGetFramebufferAttachmentParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetFramebufferAttachmentParameteriv",params);
    }
    F(target_, attachment_, pname_, params_);
    return;
}
void glGetFramebufferParameteriv ( GLenum target_, GLenum pname_, GLint * params_ ){
    static PFNGLGETFRAMEBUFFERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETFRAMEBUFFERPARAMETERIVPROC ) mygetprocaddr("glGetFramebufferParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetFramebufferParameteriv",params);
    }
    F(target_, pname_, params_);
    return;
}
GLenum glGetGraphicsResetStatus (  ){
    static PFNGLGETGRAPHICSRESETSTATUSPROC F;
    if(!F)
        F = ( PFNGLGETGRAPHICSRESETSTATUSPROC ) mygetprocaddr("glGetGraphicsResetStatus");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glGetGraphicsResetStatus",params);
    }
    GLenum retval = F();
    return retval;
}
void glGetInteger64i_v ( GLenum target_, GLuint index_, GLint64 * data_ ){
    static PFNGLGETINTEGER64I_VPROC F;
    if(!F)
        F = ( PFNGLGETINTEGER64I_VPROC ) mygetprocaddr("glGetInteger64i_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetInteger64i_v",params);
    }
    F(target_, index_, data_);
    return;
}
void glGetInteger64v ( GLenum pname_, GLint64 * data_ ){
    static PFNGLGETINTEGER64VPROC F;
    if(!F)
        F = ( PFNGLGETINTEGER64VPROC ) mygetprocaddr("glGetInteger64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetInteger64v",params);
    }
    F(pname_, data_);
    return;
}
void glGetIntegeri_v ( GLenum target_, GLuint index_, GLint * data_ ){
    static PFNGLGETINTEGERI_VPROC F;
    if(!F)
        F = ( PFNGLGETINTEGERI_VPROC ) mygetprocaddr("glGetIntegeri_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetIntegeri_v",params);
    }
    F(target_, index_, data_);
    return;
}
void glGetIntegerv ( GLenum pname_, GLint * data_ ){
    static PFNGLGETINTEGERVPROC F;
    if(!F)
        F = ( PFNGLGETINTEGERVPROC ) mygetprocaddr("glGetIntegerv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetIntegerv",params);
    }
    F(pname_, data_);
    return;
}
void glGetInternalformati64v ( GLenum target_, GLenum internalformat_, GLenum pname_, GLsizei bufSize_, GLint64 * params_ ){
    static PFNGLGETINTERNALFORMATI64VPROC F;
    if(!F)
        F = ( PFNGLGETINTERNALFORMATI64VPROC ) mygetprocaddr("glGetInternalformati64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetInternalformati64v",params);
    }
    F(target_, internalformat_, pname_, bufSize_, params_);
    return;
}
void glGetInternalformativ ( GLenum target_, GLenum internalformat_, GLenum pname_, GLsizei bufSize_, GLint * params_ ){
    static PFNGLGETINTERNALFORMATIVPROC F;
    if(!F)
        F = ( PFNGLGETINTERNALFORMATIVPROC ) mygetprocaddr("glGetInternalformativ");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetInternalformativ",params);
    }
    F(target_, internalformat_, pname_, bufSize_, params_);
    return;
}
void glGetMultisamplefv ( GLenum pname_, GLuint index_, GLfloat * val_ ){
    static PFNGLGETMULTISAMPLEFVPROC F;
    if(!F)
        F = ( PFNGLGETMULTISAMPLEFVPROC ) mygetprocaddr("glGetMultisamplefv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) val_ );
        for(auto& it : wildcardHooks )
            it("glGetMultisamplefv",params);
    }
    F(pname_, index_, val_);
    return;
}
void glGetNamedBufferParameteri64v ( GLuint buffer_, GLenum pname_, GLint64 * params_ ){
    static PFNGLGETNAMEDBUFFERPARAMETERI64VPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDBUFFERPARAMETERI64VPROC ) mygetprocaddr("glGetNamedBufferParameteri64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedBufferParameteri64v",params);
    }
    F(buffer_, pname_, params_);
    return;
}
void glGetNamedBufferParameteriv ( GLuint buffer_, GLenum pname_, GLint * params_ ){
    static PFNGLGETNAMEDBUFFERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDBUFFERPARAMETERIVPROC ) mygetprocaddr("glGetNamedBufferParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedBufferParameteriv",params);
    }
    F(buffer_, pname_, params_);
    return;
}
void glGetNamedBufferPointerv ( GLuint buffer_, GLenum pname_, void ** params_ ){
    static PFNGLGETNAMEDBUFFERPOINTERVPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDBUFFERPOINTERVPROC ) mygetprocaddr("glGetNamedBufferPointerv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedBufferPointerv",params);
    }
    F(buffer_, pname_, params_);
    return;
}
void glGetNamedBufferSubData ( GLuint buffer_, GLintptr offset_, GLsizeiptr size_, void * data_ ){
    static PFNGLGETNAMEDBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDBUFFERSUBDATAPROC ) mygetprocaddr("glGetNamedBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedBufferSubData",params);
    }
    F(buffer_, offset_, size_, data_);
    return;
}
void glGetNamedFramebufferAttachmentParameteriv ( GLuint framebuffer_, GLenum attachment_, GLenum pname_, GLint * params_ ){
    static PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC ) mygetprocaddr("glGetNamedFramebufferAttachmentParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedFramebufferAttachmentParameteriv",params);
    }
    F(framebuffer_, attachment_, pname_, params_);
    return;
}
void glGetNamedFramebufferParameteriv ( GLuint framebuffer_, GLenum pname_, GLint * param_ ){
    static PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC ) mygetprocaddr("glGetNamedFramebufferParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedFramebufferParameteriv",params);
    }
    F(framebuffer_, pname_, param_);
    return;
}
void glGetNamedRenderbufferParameteriv ( GLuint renderbuffer_, GLenum pname_, GLint * params_ ){
    static PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC ) mygetprocaddr("glGetNamedRenderbufferParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) renderbuffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetNamedRenderbufferParameteriv",params);
    }
    F(renderbuffer_, pname_, params_);
    return;
}
void glGetObjectLabel ( GLenum identifier_, GLuint name_, GLsizei bufSize_, GLsizei * length_, GLchar * label_ ){
    static PFNGLGETOBJECTLABELPROC F;
    if(!F)
        F = ( PFNGLGETOBJECTLABELPROC ) mygetprocaddr("glGetObjectLabel");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) identifier_ );
        params.push_back( (uint64_t) name_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) label_ );
        for(auto& it : wildcardHooks )
            it("glGetObjectLabel",params);
    }
    F(identifier_, name_, bufSize_, length_, label_);
    return;
}
void glGetObjectPtrLabel ( const void * ptr_, GLsizei bufSize_, GLsizei * length_, GLchar * label_ ){
    static PFNGLGETOBJECTPTRLABELPROC F;
    if(!F)
        F = ( PFNGLGETOBJECTPTRLABELPROC ) mygetprocaddr("glGetObjectPtrLabel");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) ptr_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) label_ );
        for(auto& it : wildcardHooks )
            it("glGetObjectPtrLabel",params);
    }
    F(ptr_, bufSize_, length_, label_);
    return;
}
void glGetProgramBinary ( GLuint program_, GLsizei bufSize_, GLsizei * length_, GLenum * binaryFormat_, void * binary_ ){
    static PFNGLGETPROGRAMBINARYPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMBINARYPROC ) mygetprocaddr("glGetProgramBinary");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) binaryFormat_ );
        params.push_back( (uint64_t) binary_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramBinary",params);
    }
    F(program_, bufSize_, length_, binaryFormat_, binary_);
    return;
}
void glGetProgramInfoLog ( GLuint program_, GLsizei bufSize_, GLsizei * length_, GLchar * infoLog_ ){
    static PFNGLGETPROGRAMINFOLOGPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMINFOLOGPROC ) mygetprocaddr("glGetProgramInfoLog");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) infoLog_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramInfoLog",params);
    }
    F(program_, bufSize_, length_, infoLog_);
    return;
}
void glGetProgramInterfaceiv ( GLuint program_, GLenum programInterface_, GLenum pname_, GLint * params_ ){
    static PFNGLGETPROGRAMINTERFACEIVPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMINTERFACEIVPROC ) mygetprocaddr("glGetProgramInterfaceiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) programInterface_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramInterfaceiv",params);
    }
    F(program_, programInterface_, pname_, params_);
    return;
}
void glGetProgramPipelineInfoLog ( GLuint pipeline_, GLsizei bufSize_, GLsizei * length_, GLchar * infoLog_ ){
    static PFNGLGETPROGRAMPIPELINEINFOLOGPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMPIPELINEINFOLOGPROC ) mygetprocaddr("glGetProgramPipelineInfoLog");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) infoLog_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramPipelineInfoLog",params);
    }
    F(pipeline_, bufSize_, length_, infoLog_);
    return;
}
void glGetProgramPipelineiv ( GLuint pipeline_, GLenum pname_, GLint * params_ ){
    static PFNGLGETPROGRAMPIPELINEIVPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMPIPELINEIVPROC ) mygetprocaddr("glGetProgramPipelineiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramPipelineiv",params);
    }
    F(pipeline_, pname_, params_);
    return;
}
GLuint glGetProgramResourceIndex ( GLuint program_, GLenum programInterface_, const GLchar * name_ ){
    static PFNGLGETPROGRAMRESOURCEINDEXPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMRESOURCEINDEXPROC ) mygetprocaddr("glGetProgramResourceIndex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) programInterface_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramResourceIndex",params);
    }
    GLuint retval = F(program_, programInterface_, name_);
    return retval;
}
GLint glGetProgramResourceLocation ( GLuint program_, GLenum programInterface_, const GLchar * name_ ){
    static PFNGLGETPROGRAMRESOURCELOCATIONPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMRESOURCELOCATIONPROC ) mygetprocaddr("glGetProgramResourceLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) programInterface_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramResourceLocation",params);
    }
    GLint retval = F(program_, programInterface_, name_);
    return retval;
}
GLint glGetProgramResourceLocationIndex ( GLuint program_, GLenum programInterface_, const GLchar * name_ ){
    static PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC ) mygetprocaddr("glGetProgramResourceLocationIndex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) programInterface_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramResourceLocationIndex",params);
    }
    GLint retval = F(program_, programInterface_, name_);
    return retval;
}
void glGetProgramResourceName ( GLuint program_, GLenum programInterface_, GLuint index_, GLsizei bufSize_, GLsizei * length_, GLchar * name_ ){
    static PFNGLGETPROGRAMRESOURCENAMEPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMRESOURCENAMEPROC ) mygetprocaddr("glGetProgramResourceName");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) programInterface_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramResourceName",params);
    }
    F(program_, programInterface_, index_, bufSize_, length_, name_);
    return;
}
void glGetProgramResourceiv ( GLuint program_, GLenum programInterface_, GLuint index_, GLsizei propCount_, const GLenum * props_, GLsizei bufSize_, GLsizei * length_, GLint * params_ ){
    static PFNGLGETPROGRAMRESOURCEIVPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMRESOURCEIVPROC ) mygetprocaddr("glGetProgramResourceiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) programInterface_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) propCount_ );
        params.push_back( (uint64_t) props_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramResourceiv",params);
    }
    F(program_, programInterface_, index_, propCount_, props_, bufSize_, length_, params_);
    return;
}
void glGetProgramStageiv ( GLuint program_, GLenum shadertype_, GLenum pname_, GLint * values_ ){
    static PFNGLGETPROGRAMSTAGEIVPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMSTAGEIVPROC ) mygetprocaddr("glGetProgramStageiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) values_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramStageiv",params);
    }
    F(program_, shadertype_, pname_, values_);
    return;
}
void glGetProgramiv ( GLuint program_, GLenum pname_, GLint * params_ ){
    static PFNGLGETPROGRAMIVPROC F;
    if(!F)
        F = ( PFNGLGETPROGRAMIVPROC ) mygetprocaddr("glGetProgramiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetProgramiv",params);
    }
    F(program_, pname_, params_);
    return;
}
void glGetQueryBufferObjecti64v ( GLuint id_, GLuint buffer_, GLenum pname_, GLintptr offset_ ){
    static PFNGLGETQUERYBUFFEROBJECTI64VPROC F;
    if(!F)
        F = ( PFNGLGETQUERYBUFFEROBJECTI64VPROC ) mygetprocaddr("glGetQueryBufferObjecti64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) offset_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryBufferObjecti64v",params);
    }
    F(id_, buffer_, pname_, offset_);
    return;
}
void glGetQueryBufferObjectiv ( GLuint id_, GLuint buffer_, GLenum pname_, GLintptr offset_ ){
    static PFNGLGETQUERYBUFFEROBJECTIVPROC F;
    if(!F)
        F = ( PFNGLGETQUERYBUFFEROBJECTIVPROC ) mygetprocaddr("glGetQueryBufferObjectiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) offset_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryBufferObjectiv",params);
    }
    F(id_, buffer_, pname_, offset_);
    return;
}
void glGetQueryBufferObjectui64v ( GLuint id_, GLuint buffer_, GLenum pname_, GLintptr offset_ ){
    static PFNGLGETQUERYBUFFEROBJECTUI64VPROC F;
    if(!F)
        F = ( PFNGLGETQUERYBUFFEROBJECTUI64VPROC ) mygetprocaddr("glGetQueryBufferObjectui64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) offset_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryBufferObjectui64v",params);
    }
    F(id_, buffer_, pname_, offset_);
    return;
}
void glGetQueryBufferObjectuiv ( GLuint id_, GLuint buffer_, GLenum pname_, GLintptr offset_ ){
    static PFNGLGETQUERYBUFFEROBJECTUIVPROC F;
    if(!F)
        F = ( PFNGLGETQUERYBUFFEROBJECTUIVPROC ) mygetprocaddr("glGetQueryBufferObjectuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) offset_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryBufferObjectuiv",params);
    }
    F(id_, buffer_, pname_, offset_);
    return;
}
void glGetQueryIndexediv ( GLenum target_, GLuint index_, GLenum pname_, GLint * params_ ){
    static PFNGLGETQUERYINDEXEDIVPROC F;
    if(!F)
        F = ( PFNGLGETQUERYINDEXEDIVPROC ) mygetprocaddr("glGetQueryIndexediv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryIndexediv",params);
    }
    F(target_, index_, pname_, params_);
    return;
}
void glGetQueryObjecti64v ( GLuint id_, GLenum pname_, GLint64 * params_ ){
    static PFNGLGETQUERYOBJECTI64VPROC F;
    if(!F)
        F = ( PFNGLGETQUERYOBJECTI64VPROC ) mygetprocaddr("glGetQueryObjecti64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryObjecti64v",params);
    }
    F(id_, pname_, params_);
    return;
}
void glGetQueryObjectiv ( GLuint id_, GLenum pname_, GLint * params_ ){
    static PFNGLGETQUERYOBJECTIVPROC F;
    if(!F)
        F = ( PFNGLGETQUERYOBJECTIVPROC ) mygetprocaddr("glGetQueryObjectiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryObjectiv",params);
    }
    F(id_, pname_, params_);
    return;
}
void glGetQueryObjectui64v ( GLuint id_, GLenum pname_, GLuint64 * params_ ){
    static PFNGLGETQUERYOBJECTUI64VPROC F;
    if(!F)
        F = ( PFNGLGETQUERYOBJECTUI64VPROC ) mygetprocaddr("glGetQueryObjectui64v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryObjectui64v",params);
    }
    F(id_, pname_, params_);
    return;
}
void glGetQueryObjectuiv ( GLuint id_, GLenum pname_, GLuint * params_ ){
    static PFNGLGETQUERYOBJECTUIVPROC F;
    if(!F)
        F = ( PFNGLGETQUERYOBJECTUIVPROC ) mygetprocaddr("glGetQueryObjectuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryObjectuiv",params);
    }
    F(id_, pname_, params_);
    return;
}
void glGetQueryiv ( GLenum target_, GLenum pname_, GLint * params_ ){
    static PFNGLGETQUERYIVPROC F;
    if(!F)
        F = ( PFNGLGETQUERYIVPROC ) mygetprocaddr("glGetQueryiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetQueryiv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetRenderbufferParameteriv ( GLenum target_, GLenum pname_, GLint * params_ ){
    static PFNGLGETRENDERBUFFERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETRENDERBUFFERPARAMETERIVPROC ) mygetprocaddr("glGetRenderbufferParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetRenderbufferParameteriv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetSamplerParameterIiv ( GLuint sampler_, GLenum pname_, GLint * params_ ){
    static PFNGLGETSAMPLERPARAMETERIIVPROC F;
    if(!F)
        F = ( PFNGLGETSAMPLERPARAMETERIIVPROC ) mygetprocaddr("glGetSamplerParameterIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetSamplerParameterIiv",params);
    }
    F(sampler_, pname_, params_);
    return;
}
void glGetSamplerParameterIuiv ( GLuint sampler_, GLenum pname_, GLuint * params_ ){
    static PFNGLGETSAMPLERPARAMETERIUIVPROC F;
    if(!F)
        F = ( PFNGLGETSAMPLERPARAMETERIUIVPROC ) mygetprocaddr("glGetSamplerParameterIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetSamplerParameterIuiv",params);
    }
    F(sampler_, pname_, params_);
    return;
}
void glGetSamplerParameterfv ( GLuint sampler_, GLenum pname_, GLfloat * params_ ){
    static PFNGLGETSAMPLERPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLGETSAMPLERPARAMETERFVPROC ) mygetprocaddr("glGetSamplerParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetSamplerParameterfv",params);
    }
    F(sampler_, pname_, params_);
    return;
}
void glGetSamplerParameteriv ( GLuint sampler_, GLenum pname_, GLint * params_ ){
    static PFNGLGETSAMPLERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETSAMPLERPARAMETERIVPROC ) mygetprocaddr("glGetSamplerParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetSamplerParameteriv",params);
    }
    F(sampler_, pname_, params_);
    return;
}
void glGetShaderInfoLog ( GLuint shader_, GLsizei bufSize_, GLsizei * length_, GLchar * infoLog_ ){
    static PFNGLGETSHADERINFOLOGPROC F;
    if(!F)
        F = ( PFNGLGETSHADERINFOLOGPROC ) mygetprocaddr("glGetShaderInfoLog");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) infoLog_ );
        for(auto& it : wildcardHooks )
            it("glGetShaderInfoLog",params);
    }
    F(shader_, bufSize_, length_, infoLog_);
    return;
}
void glGetShaderPrecisionFormat ( GLenum shadertype_, GLenum precisiontype_, GLint * range_, GLint * precision_ ){
    static PFNGLGETSHADERPRECISIONFORMATPROC F;
    if(!F)
        F = ( PFNGLGETSHADERPRECISIONFORMATPROC ) mygetprocaddr("glGetShaderPrecisionFormat");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) precisiontype_ );
        params.push_back( (uint64_t) range_ );
        params.push_back( (uint64_t) precision_ );
        for(auto& it : wildcardHooks )
            it("glGetShaderPrecisionFormat",params);
    }
    F(shadertype_, precisiontype_, range_, precision_);
    return;
}
void glGetShaderSource ( GLuint shader_, GLsizei bufSize_, GLsizei * length_, GLchar * source_ ){
    static PFNGLGETSHADERSOURCEPROC F;
    if(!F)
        F = ( PFNGLGETSHADERSOURCEPROC ) mygetprocaddr("glGetShaderSource");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) source_ );
        for(auto& it : wildcardHooks )
            it("glGetShaderSource",params);
    }
    F(shader_, bufSize_, length_, source_);
    return;
}
void glGetShaderiv ( GLuint shader_, GLenum pname_, GLint * params_ ){
    static PFNGLGETSHADERIVPROC F;
    if(!F)
        F = ( PFNGLGETSHADERIVPROC ) mygetprocaddr("glGetShaderiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetShaderiv",params);
    }
    F(shader_, pname_, params_);
    return;
}
const GLubyte * glGetString ( GLenum name_ ){
    static PFNGLGETSTRINGPROC F;
    if(!F)
        F = ( PFNGLGETSTRINGPROC ) mygetprocaddr("glGetString");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetString",params);
    }
    const GLubyte * retval = F(name_);
    return retval;
}
const GLubyte * glGetStringi ( GLenum name_, GLuint index_ ){
    static PFNGLGETSTRINGIPROC F;
    if(!F)
        F = ( PFNGLGETSTRINGIPROC ) mygetprocaddr("glGetStringi");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) name_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glGetStringi",params);
    }
    const GLubyte * retval = F(name_, index_);
    return retval;
}
GLuint glGetSubroutineIndex ( GLuint program_, GLenum shadertype_, const GLchar * name_ ){
    static PFNGLGETSUBROUTINEINDEXPROC F;
    if(!F)
        F = ( PFNGLGETSUBROUTINEINDEXPROC ) mygetprocaddr("glGetSubroutineIndex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetSubroutineIndex",params);
    }
    GLuint retval = F(program_, shadertype_, name_);
    return retval;
}
GLint glGetSubroutineUniformLocation ( GLuint program_, GLenum shadertype_, const GLchar * name_ ){
    static PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC F;
    if(!F)
        F = ( PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC ) mygetprocaddr("glGetSubroutineUniformLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetSubroutineUniformLocation",params);
    }
    GLint retval = F(program_, shadertype_, name_);
    return retval;
}
void glGetSynciv ( GLsync sync_, GLenum pname_, GLsizei bufSize_, GLsizei * length_, GLint * values_ ){
    static PFNGLGETSYNCIVPROC F;
    if(!F)
        F = ( PFNGLGETSYNCIVPROC ) mygetprocaddr("glGetSynciv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sync_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) values_ );
        for(auto& it : wildcardHooks )
            it("glGetSynciv",params);
    }
    F(sync_, pname_, bufSize_, length_, values_);
    return;
}
void glGetTexImage ( GLenum target_, GLint level_, GLenum format_, GLenum type_, void * pixels_ ){
    static PFNGLGETTEXIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETTEXIMAGEPROC ) mygetprocaddr("glGetTexImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetTexImage",params);
    }
    F(target_, level_, format_, type_, pixels_);
    return;
}
void glGetTexLevelParameterfv ( GLenum target_, GLint level_, GLenum pname_, GLfloat * params_ ){
    static PFNGLGETTEXLEVELPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLGETTEXLEVELPARAMETERFVPROC ) mygetprocaddr("glGetTexLevelParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTexLevelParameterfv",params);
    }
    F(target_, level_, pname_, params_);
    return;
}
void glGetTexLevelParameteriv ( GLenum target_, GLint level_, GLenum pname_, GLint * params_ ){
    static PFNGLGETTEXLEVELPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXLEVELPARAMETERIVPROC ) mygetprocaddr("glGetTexLevelParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTexLevelParameteriv",params);
    }
    F(target_, level_, pname_, params_);
    return;
}
void glGetTexParameterIiv ( GLenum target_, GLenum pname_, GLint * params_ ){
    static PFNGLGETTEXPARAMETERIIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXPARAMETERIIVPROC ) mygetprocaddr("glGetTexParameterIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTexParameterIiv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetTexParameterIuiv ( GLenum target_, GLenum pname_, GLuint * params_ ){
    static PFNGLGETTEXPARAMETERIUIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXPARAMETERIUIVPROC ) mygetprocaddr("glGetTexParameterIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTexParameterIuiv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetTexParameterfv ( GLenum target_, GLenum pname_, GLfloat * params_ ){
    static PFNGLGETTEXPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLGETTEXPARAMETERFVPROC ) mygetprocaddr("glGetTexParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTexParameterfv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetTexParameteriv ( GLenum target_, GLenum pname_, GLint * params_ ){
    static PFNGLGETTEXPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXPARAMETERIVPROC ) mygetprocaddr("glGetTexParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTexParameteriv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glGetTextureImage ( GLuint texture_, GLint level_, GLenum format_, GLenum type_, GLsizei bufSize_, void * pixels_ ){
    static PFNGLGETTEXTUREIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETTEXTUREIMAGEPROC ) mygetprocaddr("glGetTextureImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureImage",params);
    }
    F(texture_, level_, format_, type_, bufSize_, pixels_);
    return;
}
void glGetTextureLevelParameterfv ( GLuint texture_, GLint level_, GLenum pname_, GLfloat * params_ ){
    static PFNGLGETTEXTURELEVELPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLGETTEXTURELEVELPARAMETERFVPROC ) mygetprocaddr("glGetTextureLevelParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureLevelParameterfv",params);
    }
    F(texture_, level_, pname_, params_);
    return;
}
void glGetTextureLevelParameteriv ( GLuint texture_, GLint level_, GLenum pname_, GLint * params_ ){
    static PFNGLGETTEXTURELEVELPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXTURELEVELPARAMETERIVPROC ) mygetprocaddr("glGetTextureLevelParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureLevelParameteriv",params);
    }
    F(texture_, level_, pname_, params_);
    return;
}
void glGetTextureParameterIiv ( GLuint texture_, GLenum pname_, GLint * params_ ){
    static PFNGLGETTEXTUREPARAMETERIIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXTUREPARAMETERIIVPROC ) mygetprocaddr("glGetTextureParameterIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureParameterIiv",params);
    }
    F(texture_, pname_, params_);
    return;
}
void glGetTextureParameterIuiv ( GLuint texture_, GLenum pname_, GLuint * params_ ){
    static PFNGLGETTEXTUREPARAMETERIUIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXTUREPARAMETERIUIVPROC ) mygetprocaddr("glGetTextureParameterIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureParameterIuiv",params);
    }
    F(texture_, pname_, params_);
    return;
}
void glGetTextureParameterfv ( GLuint texture_, GLenum pname_, GLfloat * params_ ){
    static PFNGLGETTEXTUREPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLGETTEXTUREPARAMETERFVPROC ) mygetprocaddr("glGetTextureParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureParameterfv",params);
    }
    F(texture_, pname_, params_);
    return;
}
void glGetTextureParameteriv ( GLuint texture_, GLenum pname_, GLint * params_ ){
    static PFNGLGETTEXTUREPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLGETTEXTUREPARAMETERIVPROC ) mygetprocaddr("glGetTextureParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureParameteriv",params);
    }
    F(texture_, pname_, params_);
    return;
}
void glGetTextureSubImage ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLenum format_, GLenum type_, GLsizei bufSize_, void * pixels_ ){
    static PFNGLGETTEXTURESUBIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETTEXTURESUBIMAGEPROC ) mygetprocaddr("glGetTextureSubImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetTextureSubImage",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, format_, type_, bufSize_, pixels_);
    return;
}
void glGetTransformFeedbackVarying ( GLuint program_, GLuint index_, GLsizei bufSize_, GLsizei * length_, GLsizei * size_, GLenum * type_, GLchar * name_ ){
    static PFNGLGETTRANSFORMFEEDBACKVARYINGPROC F;
    if(!F)
        F = ( PFNGLGETTRANSFORMFEEDBACKVARYINGPROC ) mygetprocaddr("glGetTransformFeedbackVarying");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetTransformFeedbackVarying",params);
    }
    F(program_, index_, bufSize_, length_, size_, type_, name_);
    return;
}
void glGetTransformFeedbacki64_v ( GLuint xfb_, GLenum pname_, GLuint index_, GLint64 * param_ ){
    static PFNGLGETTRANSFORMFEEDBACKI64_VPROC F;
    if(!F)
        F = ( PFNGLGETTRANSFORMFEEDBACKI64_VPROC ) mygetprocaddr("glGetTransformFeedbacki64_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) xfb_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetTransformFeedbacki64_v",params);
    }
    F(xfb_, pname_, index_, param_);
    return;
}
void glGetTransformFeedbacki_v ( GLuint xfb_, GLenum pname_, GLuint index_, GLint * param_ ){
    static PFNGLGETTRANSFORMFEEDBACKI_VPROC F;
    if(!F)
        F = ( PFNGLGETTRANSFORMFEEDBACKI_VPROC ) mygetprocaddr("glGetTransformFeedbacki_v");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) xfb_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetTransformFeedbacki_v",params);
    }
    F(xfb_, pname_, index_, param_);
    return;
}
void glGetTransformFeedbackiv ( GLuint xfb_, GLenum pname_, GLint * param_ ){
    static PFNGLGETTRANSFORMFEEDBACKIVPROC F;
    if(!F)
        F = ( PFNGLGETTRANSFORMFEEDBACKIVPROC ) mygetprocaddr("glGetTransformFeedbackiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) xfb_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetTransformFeedbackiv",params);
    }
    F(xfb_, pname_, param_);
    return;
}
GLuint glGetUniformBlockIndex ( GLuint program_, const GLchar * uniformBlockName_ ){
    static PFNGLGETUNIFORMBLOCKINDEXPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMBLOCKINDEXPROC ) mygetprocaddr("glGetUniformBlockIndex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformBlockName_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformBlockIndex",params);
    }
    GLuint retval = F(program_, uniformBlockName_);
    return retval;
}
void glGetUniformIndices ( GLuint program_, GLsizei uniformCount_, const GLchar ** uniformNames_, GLuint * uniformIndices_ ){
    static PFNGLGETUNIFORMINDICESPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMINDICESPROC ) mygetprocaddr("glGetUniformIndices");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformCount_ );
        params.push_back( (uint64_t) uniformNames_ );
        params.push_back( (uint64_t) uniformIndices_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformIndices",params);
    }
    F(program_, uniformCount_, uniformNames_, uniformIndices_);
    return;
}
GLint glGetUniformLocation ( GLuint program_, const GLchar * name_ ){
    static PFNGLGETUNIFORMLOCATIONPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMLOCATIONPROC ) mygetprocaddr("glGetUniformLocation");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) name_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformLocation",params);
    }
    GLint retval = F(program_, name_);
    return retval;
}
void glGetUniformSubroutineuiv ( GLenum shadertype_, GLint location_, GLuint * params_ ){
    static PFNGLGETUNIFORMSUBROUTINEUIVPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMSUBROUTINEUIVPROC ) mygetprocaddr("glGetUniformSubroutineuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformSubroutineuiv",params);
    }
    F(shadertype_, location_, params_);
    return;
}
void glGetUniformdv ( GLuint program_, GLint location_, GLdouble * params_ ){
    static PFNGLGETUNIFORMDVPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMDVPROC ) mygetprocaddr("glGetUniformdv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformdv",params);
    }
    F(program_, location_, params_);
    return;
}
void glGetUniformfv ( GLuint program_, GLint location_, GLfloat * params_ ){
    static PFNGLGETUNIFORMFVPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMFVPROC ) mygetprocaddr("glGetUniformfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformfv",params);
    }
    F(program_, location_, params_);
    return;
}
void glGetUniformiv ( GLuint program_, GLint location_, GLint * params_ ){
    static PFNGLGETUNIFORMIVPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMIVPROC ) mygetprocaddr("glGetUniformiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformiv",params);
    }
    F(program_, location_, params_);
    return;
}
void glGetUniformuiv ( GLuint program_, GLint location_, GLuint * params_ ){
    static PFNGLGETUNIFORMUIVPROC F;
    if(!F)
        F = ( PFNGLGETUNIFORMUIVPROC ) mygetprocaddr("glGetUniformuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetUniformuiv",params);
    }
    F(program_, location_, params_);
    return;
}
void glGetVertexArrayIndexed64iv ( GLuint vaobj_, GLuint index_, GLenum pname_, GLint64 * param_ ){
    static PFNGLGETVERTEXARRAYINDEXED64IVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXARRAYINDEXED64IVPROC ) mygetprocaddr("glGetVertexArrayIndexed64iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexArrayIndexed64iv",params);
    }
    F(vaobj_, index_, pname_, param_);
    return;
}
void glGetVertexArrayIndexediv ( GLuint vaobj_, GLuint index_, GLenum pname_, GLint * param_ ){
    static PFNGLGETVERTEXARRAYINDEXEDIVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXARRAYINDEXEDIVPROC ) mygetprocaddr("glGetVertexArrayIndexediv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexArrayIndexediv",params);
    }
    F(vaobj_, index_, pname_, param_);
    return;
}
void glGetVertexArrayiv ( GLuint vaobj_, GLenum pname_, GLint * param_ ){
    static PFNGLGETVERTEXARRAYIVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXARRAYIVPROC ) mygetprocaddr("glGetVertexArrayiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexArrayiv",params);
    }
    F(vaobj_, pname_, param_);
    return;
}
void glGetVertexAttribIiv ( GLuint index_, GLenum pname_, GLint * params_ ){
    static PFNGLGETVERTEXATTRIBIIVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBIIVPROC ) mygetprocaddr("glGetVertexAttribIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribIiv",params);
    }
    F(index_, pname_, params_);
    return;
}
void glGetVertexAttribIuiv ( GLuint index_, GLenum pname_, GLuint * params_ ){
    static PFNGLGETVERTEXATTRIBIUIVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBIUIVPROC ) mygetprocaddr("glGetVertexAttribIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribIuiv",params);
    }
    F(index_, pname_, params_);
    return;
}
void glGetVertexAttribLdv ( GLuint index_, GLenum pname_, GLdouble * params_ ){
    static PFNGLGETVERTEXATTRIBLDVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBLDVPROC ) mygetprocaddr("glGetVertexAttribLdv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribLdv",params);
    }
    F(index_, pname_, params_);
    return;
}
void glGetVertexAttribPointerv ( GLuint index_, GLenum pname_, void ** pointer_ ){
    static PFNGLGETVERTEXATTRIBPOINTERVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBPOINTERVPROC ) mygetprocaddr("glGetVertexAttribPointerv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) pointer_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribPointerv",params);
    }
    F(index_, pname_, pointer_);
    return;
}
void glGetVertexAttribdv ( GLuint index_, GLenum pname_, GLdouble * params_ ){
    static PFNGLGETVERTEXATTRIBDVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBDVPROC ) mygetprocaddr("glGetVertexAttribdv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribdv",params);
    }
    F(index_, pname_, params_);
    return;
}
void glGetVertexAttribfv ( GLuint index_, GLenum pname_, GLfloat * params_ ){
    static PFNGLGETVERTEXATTRIBFVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBFVPROC ) mygetprocaddr("glGetVertexAttribfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribfv",params);
    }
    F(index_, pname_, params_);
    return;
}
void glGetVertexAttribiv ( GLuint index_, GLenum pname_, GLint * params_ ){
    static PFNGLGETVERTEXATTRIBIVPROC F;
    if(!F)
        F = ( PFNGLGETVERTEXATTRIBIVPROC ) mygetprocaddr("glGetVertexAttribiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetVertexAttribiv",params);
    }
    F(index_, pname_, params_);
    return;
}
void glGetnCompressedTexImage ( GLenum target_, GLint lod_, GLsizei bufSize_, void * pixels_ ){
    static PFNGLGETNCOMPRESSEDTEXIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETNCOMPRESSEDTEXIMAGEPROC ) mygetprocaddr("glGetnCompressedTexImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) lod_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetnCompressedTexImage",params);
    }
    F(target_, lod_, bufSize_, pixels_);
    return;
}
void glGetnTexImage ( GLenum target_, GLint level_, GLenum format_, GLenum type_, GLsizei bufSize_, void * pixels_ ){
    static PFNGLGETNTEXIMAGEPROC F;
    if(!F)
        F = ( PFNGLGETNTEXIMAGEPROC ) mygetprocaddr("glGetnTexImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glGetnTexImage",params);
    }
    F(target_, level_, format_, type_, bufSize_, pixels_);
    return;
}
void glGetnUniformdv ( GLuint program_, GLint location_, GLsizei bufSize_, GLdouble * params_ ){
    static PFNGLGETNUNIFORMDVPROC F;
    if(!F)
        F = ( PFNGLGETNUNIFORMDVPROC ) mygetprocaddr("glGetnUniformdv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetnUniformdv",params);
    }
    F(program_, location_, bufSize_, params_);
    return;
}
void glGetnUniformfv ( GLuint program_, GLint location_, GLsizei bufSize_, GLfloat * params_ ){
    static PFNGLGETNUNIFORMFVPROC F;
    if(!F)
        F = ( PFNGLGETNUNIFORMFVPROC ) mygetprocaddr("glGetnUniformfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetnUniformfv",params);
    }
    F(program_, location_, bufSize_, params_);
    return;
}
void glGetnUniformiv ( GLuint program_, GLint location_, GLsizei bufSize_, GLint * params_ ){
    static PFNGLGETNUNIFORMIVPROC F;
    if(!F)
        F = ( PFNGLGETNUNIFORMIVPROC ) mygetprocaddr("glGetnUniformiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetnUniformiv",params);
    }
    F(program_, location_, bufSize_, params_);
    return;
}
void glGetnUniformuiv ( GLuint program_, GLint location_, GLsizei bufSize_, GLuint * params_ ){
    static PFNGLGETNUNIFORMUIVPROC F;
    if(!F)
        F = ( PFNGLGETNUNIFORMUIVPROC ) mygetprocaddr("glGetnUniformuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glGetnUniformuiv",params);
    }
    F(program_, location_, bufSize_, params_);
    return;
}
void glHint ( GLenum target_, GLenum mode_ ){
    static PFNGLHINTPROC F;
    if(!F)
        F = ( PFNGLHINTPROC ) mygetprocaddr("glHint");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glHint",params);
    }
    F(target_, mode_);
    return;
}
void glInvalidateBufferData ( GLuint buffer_ ){
    static PFNGLINVALIDATEBUFFERDATAPROC F;
    if(!F)
        F = ( PFNGLINVALIDATEBUFFERDATAPROC ) mygetprocaddr("glInvalidateBufferData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateBufferData",params);
    }
    F(buffer_);
    return;
}
void glInvalidateBufferSubData ( GLuint buffer_, GLintptr offset_, GLsizeiptr length_ ){
    static PFNGLINVALIDATEBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLINVALIDATEBUFFERSUBDATAPROC ) mygetprocaddr("glInvalidateBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) length_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateBufferSubData",params);
    }
    F(buffer_, offset_, length_);
    return;
}
void glInvalidateFramebuffer ( GLenum target_, GLsizei numAttachments_, const GLenum * attachments_ ){
    static PFNGLINVALIDATEFRAMEBUFFERPROC F;
    if(!F)
        F = ( PFNGLINVALIDATEFRAMEBUFFERPROC ) mygetprocaddr("glInvalidateFramebuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) numAttachments_ );
        params.push_back( (uint64_t) attachments_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateFramebuffer",params);
    }
    F(target_, numAttachments_, attachments_);
    return;
}
void glInvalidateNamedFramebufferData ( GLuint framebuffer_, GLsizei numAttachments_, const GLenum * attachments_ ){
    static PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC F;
    if(!F)
        F = ( PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC ) mygetprocaddr("glInvalidateNamedFramebufferData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) numAttachments_ );
        params.push_back( (uint64_t) attachments_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateNamedFramebufferData",params);
    }
    F(framebuffer_, numAttachments_, attachments_);
    return;
}
void glInvalidateNamedFramebufferSubData ( GLuint framebuffer_, GLsizei numAttachments_, const GLenum * attachments_, GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC ) mygetprocaddr("glInvalidateNamedFramebufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) numAttachments_ );
        params.push_back( (uint64_t) attachments_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateNamedFramebufferSubData",params);
    }
    F(framebuffer_, numAttachments_, attachments_, x_, y_, width_, height_);
    return;
}
void glInvalidateSubFramebuffer ( GLenum target_, GLsizei numAttachments_, const GLenum * attachments_, GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLINVALIDATESUBFRAMEBUFFERPROC F;
    if(!F)
        F = ( PFNGLINVALIDATESUBFRAMEBUFFERPROC ) mygetprocaddr("glInvalidateSubFramebuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) numAttachments_ );
        params.push_back( (uint64_t) attachments_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateSubFramebuffer",params);
    }
    F(target_, numAttachments_, attachments_, x_, y_, width_, height_);
    return;
}
void glInvalidateTexImage ( GLuint texture_, GLint level_ ){
    static PFNGLINVALIDATETEXIMAGEPROC F;
    if(!F)
        F = ( PFNGLINVALIDATETEXIMAGEPROC ) mygetprocaddr("glInvalidateTexImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateTexImage",params);
    }
    F(texture_, level_);
    return;
}
void glInvalidateTexSubImage ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_ ){
    static PFNGLINVALIDATETEXSUBIMAGEPROC F;
    if(!F)
        F = ( PFNGLINVALIDATETEXSUBIMAGEPROC ) mygetprocaddr("glInvalidateTexSubImage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        for(auto& it : wildcardHooks )
            it("glInvalidateTexSubImage",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_);
    return;
}
GLboolean glIsBuffer ( GLuint buffer_ ){
    static PFNGLISBUFFERPROC F;
    if(!F)
        F = ( PFNGLISBUFFERPROC ) mygetprocaddr("glIsBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glIsBuffer",params);
    }
    GLboolean retval = F(buffer_);
    return retval;
}
GLboolean glIsEnabled ( GLenum cap_ ){
    static PFNGLISENABLEDPROC F;
    if(!F)
        F = ( PFNGLISENABLEDPROC ) mygetprocaddr("glIsEnabled");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) cap_ );
        for(auto& it : wildcardHooks )
            it("glIsEnabled",params);
    }
    GLboolean retval = F(cap_);
    return retval;
}
GLboolean glIsEnabledi ( GLenum target_, GLuint index_ ){
    static PFNGLISENABLEDIPROC F;
    if(!F)
        F = ( PFNGLISENABLEDIPROC ) mygetprocaddr("glIsEnabledi");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glIsEnabledi",params);
    }
    GLboolean retval = F(target_, index_);
    return retval;
}
GLboolean glIsFramebuffer ( GLuint framebuffer_ ){
    static PFNGLISFRAMEBUFFERPROC F;
    if(!F)
        F = ( PFNGLISFRAMEBUFFERPROC ) mygetprocaddr("glIsFramebuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        for(auto& it : wildcardHooks )
            it("glIsFramebuffer",params);
    }
    GLboolean retval = F(framebuffer_);
    return retval;
}
GLboolean glIsProgram ( GLuint program_ ){
    static PFNGLISPROGRAMPROC F;
    if(!F)
        F = ( PFNGLISPROGRAMPROC ) mygetprocaddr("glIsProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glIsProgram",params);
    }
    GLboolean retval = F(program_);
    return retval;
}
GLboolean glIsProgramPipeline ( GLuint pipeline_ ){
    static PFNGLISPROGRAMPIPELINEPROC F;
    if(!F)
        F = ( PFNGLISPROGRAMPIPELINEPROC ) mygetprocaddr("glIsProgramPipeline");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        for(auto& it : wildcardHooks )
            it("glIsProgramPipeline",params);
    }
    GLboolean retval = F(pipeline_);
    return retval;
}
GLboolean glIsQuery ( GLuint id_ ){
    static PFNGLISQUERYPROC F;
    if(!F)
        F = ( PFNGLISQUERYPROC ) mygetprocaddr("glIsQuery");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        for(auto& it : wildcardHooks )
            it("glIsQuery",params);
    }
    GLboolean retval = F(id_);
    return retval;
}
GLboolean glIsRenderbuffer ( GLuint renderbuffer_ ){
    static PFNGLISRENDERBUFFERPROC F;
    if(!F)
        F = ( PFNGLISRENDERBUFFERPROC ) mygetprocaddr("glIsRenderbuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) renderbuffer_ );
        for(auto& it : wildcardHooks )
            it("glIsRenderbuffer",params);
    }
    GLboolean retval = F(renderbuffer_);
    return retval;
}
GLboolean glIsSampler ( GLuint sampler_ ){
    static PFNGLISSAMPLERPROC F;
    if(!F)
        F = ( PFNGLISSAMPLERPROC ) mygetprocaddr("glIsSampler");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        for(auto& it : wildcardHooks )
            it("glIsSampler",params);
    }
    GLboolean retval = F(sampler_);
    return retval;
}
GLboolean glIsShader ( GLuint shader_ ){
    static PFNGLISSHADERPROC F;
    if(!F)
        F = ( PFNGLISSHADERPROC ) mygetprocaddr("glIsShader");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        for(auto& it : wildcardHooks )
            it("glIsShader",params);
    }
    GLboolean retval = F(shader_);
    return retval;
}
GLboolean glIsSync ( GLsync sync_ ){
    static PFNGLISSYNCPROC F;
    if(!F)
        F = ( PFNGLISSYNCPROC ) mygetprocaddr("glIsSync");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sync_ );
        for(auto& it : wildcardHooks )
            it("glIsSync",params);
    }
    GLboolean retval = F(sync_);
    return retval;
}
GLboolean glIsTexture ( GLuint texture_ ){
    static PFNGLISTEXTUREPROC F;
    if(!F)
        F = ( PFNGLISTEXTUREPROC ) mygetprocaddr("glIsTexture");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        for(auto& it : wildcardHooks )
            it("glIsTexture",params);
    }
    GLboolean retval = F(texture_);
    return retval;
}
GLboolean glIsTransformFeedback ( GLuint id_ ){
    static PFNGLISTRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLISTRANSFORMFEEDBACKPROC ) mygetprocaddr("glIsTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        for(auto& it : wildcardHooks )
            it("glIsTransformFeedback",params);
    }
    GLboolean retval = F(id_);
    return retval;
}
GLboolean glIsVertexArray ( GLuint array_ ){
    static PFNGLISVERTEXARRAYPROC F;
    if(!F)
        F = ( PFNGLISVERTEXARRAYPROC ) mygetprocaddr("glIsVertexArray");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) array_ );
        for(auto& it : wildcardHooks )
            it("glIsVertexArray",params);
    }
    GLboolean retval = F(array_);
    return retval;
}
void glLineWidth ( GLfloat width_ ){
    static PFNGLLINEWIDTHPROC F;
    if(!F)
        F = ( PFNGLLINEWIDTHPROC ) mygetprocaddr("glLineWidth");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) width_ );
        for(auto& it : wildcardHooks )
            it("glLineWidth",params);
    }
    F(width_);
    return;
}
void glLinkProgram ( GLuint program_ ){
    static PFNGLLINKPROGRAMPROC F;
    if(!F)
        F = ( PFNGLLINKPROGRAMPROC ) mygetprocaddr("glLinkProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glLinkProgram",params);
    }
    F(program_);
    return;
}
void glLogicOp ( GLenum opcode_ ){
    static PFNGLLOGICOPPROC F;
    if(!F)
        F = ( PFNGLLOGICOPPROC ) mygetprocaddr("glLogicOp");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) opcode_ );
        for(auto& it : wildcardHooks )
            it("glLogicOp",params);
    }
    F(opcode_);
    return;
}
void * glMapBuffer ( GLenum target_, GLenum access_ ){
    static PFNGLMAPBUFFERPROC F;
    if(!F)
        F = ( PFNGLMAPBUFFERPROC ) mygetprocaddr("glMapBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) access_ );
        for(auto& it : wildcardHooks )
            it("glMapBuffer",params);
    }
    void * retval = F(target_, access_);
    return retval;
}
void * glMapBufferRange ( GLenum target_, GLintptr offset_, GLsizeiptr length_, GLbitfield access_ ){
    static PFNGLMAPBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLMAPBUFFERRANGEPROC ) mygetprocaddr("glMapBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) access_ );
        for(auto& it : wildcardHooks )
            it("glMapBufferRange",params);
    }
    void * retval = F(target_, offset_, length_, access_);
    return retval;
}
void * glMapNamedBuffer ( GLuint buffer_, GLenum access_ ){
    static PFNGLMAPNAMEDBUFFERPROC F;
    if(!F)
        F = ( PFNGLMAPNAMEDBUFFERPROC ) mygetprocaddr("glMapNamedBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) access_ );
        for(auto& it : wildcardHooks )
            it("glMapNamedBuffer",params);
    }
    void * retval = F(buffer_, access_);
    return retval;
}
void * glMapNamedBufferRange ( GLuint buffer_, GLintptr offset_, GLsizeiptr length_, GLbitfield access_ ){
    static PFNGLMAPNAMEDBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLMAPNAMEDBUFFERRANGEPROC ) mygetprocaddr("glMapNamedBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) access_ );
        for(auto& it : wildcardHooks )
            it("glMapNamedBufferRange",params);
    }
    void * retval = F(buffer_, offset_, length_, access_);
    return retval;
}
void glMemoryBarrier ( GLbitfield barriers_ ){
    static PFNGLMEMORYBARRIERPROC F;
    if(!F)
        F = ( PFNGLMEMORYBARRIERPROC ) mygetprocaddr("glMemoryBarrier");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) barriers_ );
        for(auto& it : wildcardHooks )
            it("glMemoryBarrier",params);
    }
    F(barriers_);
    return;
}
void glMemoryBarrierByRegion ( GLbitfield barriers_ ){
    static PFNGLMEMORYBARRIERBYREGIONPROC F;
    if(!F)
        F = ( PFNGLMEMORYBARRIERBYREGIONPROC ) mygetprocaddr("glMemoryBarrierByRegion");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) barriers_ );
        for(auto& it : wildcardHooks )
            it("glMemoryBarrierByRegion",params);
    }
    F(barriers_);
    return;
}
void glMinSampleShading ( GLfloat value_ ){
    static PFNGLMINSAMPLESHADINGPROC F;
    if(!F)
        F = ( PFNGLMINSAMPLESHADINGPROC ) mygetprocaddr("glMinSampleShading");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glMinSampleShading",params);
    }
    F(value_);
    return;
}
void glMultiDrawArrays ( GLenum mode_, const GLint * first_, const GLsizei * count_, GLsizei drawcount_ ){
    static PFNGLMULTIDRAWARRAYSPROC F;
    if(!F)
        F = ( PFNGLMULTIDRAWARRAYSPROC ) mygetprocaddr("glMultiDrawArrays");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) drawcount_ );
        for(auto& it : drawCallHooks )
            it("glMultiDrawArrays",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) drawcount_ );
        for(auto& it : wildcardHooks )
            it("glMultiDrawArrays",params);
    }
    F(mode_, first_, count_, drawcount_);
    return;
}
void glMultiDrawArraysIndirect ( GLenum mode_, const void * indirect_, GLsizei drawcount_, GLsizei stride_ ){
    static PFNGLMULTIDRAWARRAYSINDIRECTPROC F;
    if(!F)
        F = ( PFNGLMULTIDRAWARRAYSINDIRECTPROC ) mygetprocaddr("glMultiDrawArraysIndirect");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) indirect_ );
        params.push_back( (uint64_t) drawcount_ );
        params.push_back( (uint64_t) stride_ );
        for(auto& it : drawCallHooks )
            it("glMultiDrawArraysIndirect",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) indirect_ );
        params.push_back( (uint64_t) drawcount_ );
        params.push_back( (uint64_t) stride_ );
        for(auto& it : wildcardHooks )
            it("glMultiDrawArraysIndirect",params);
    }
    F(mode_, indirect_, drawcount_, stride_);
    return;
}
void glMultiDrawElements ( GLenum mode_, const GLsizei * count_, GLenum type_, const void ** indices_, GLsizei drawcount_ ){
    static PFNGLMULTIDRAWELEMENTSPROC F;
    if(!F)
        F = ( PFNGLMULTIDRAWELEMENTSPROC ) mygetprocaddr("glMultiDrawElements");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) drawcount_ );
        for(auto& it : drawCallHooks )
            it("glMultiDrawElements",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) drawcount_ );
        for(auto& it : wildcardHooks )
            it("glMultiDrawElements",params);
    }
    F(mode_, count_, type_, indices_, drawcount_);
    return;
}
void glMultiDrawElementsBaseVertex ( GLenum mode_, const GLsizei * count_, GLenum type_, const void ** indices_, GLsizei drawcount_, const GLint * basevertex_ ){
    static PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC F;
    if(!F)
        F = ( PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC ) mygetprocaddr("glMultiDrawElementsBaseVertex");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) drawcount_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : drawCallHooks )
            it("glMultiDrawElementsBaseVertex",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indices_ );
        params.push_back( (uint64_t) drawcount_ );
        params.push_back( (uint64_t) basevertex_ );
        for(auto& it : wildcardHooks )
            it("glMultiDrawElementsBaseVertex",params);
    }
    F(mode_, count_, type_, indices_, drawcount_, basevertex_);
    return;
}
void glMultiDrawElementsIndirect ( GLenum mode_, GLenum type_, const void * indirect_, GLsizei drawcount_, GLsizei stride_ ){
    static PFNGLMULTIDRAWELEMENTSINDIRECTPROC F;
    if(!F)
        F = ( PFNGLMULTIDRAWELEMENTSINDIRECTPROC ) mygetprocaddr("glMultiDrawElementsIndirect");
    if( !drawCallHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indirect_ );
        params.push_back( (uint64_t) drawcount_ );
        params.push_back( (uint64_t) stride_ );
        for(auto& it : drawCallHooks )
            it("glMultiDrawElementsIndirect",params);
    }
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) indirect_ );
        params.push_back( (uint64_t) drawcount_ );
        params.push_back( (uint64_t) stride_ );
        for(auto& it : wildcardHooks )
            it("glMultiDrawElementsIndirect",params);
    }
    F(mode_, type_, indirect_, drawcount_, stride_);
    return;
}
void glNamedBufferData ( GLuint buffer_, GLsizeiptr size_, const void * data_, GLenum usage_ ){
    static PFNGLNAMEDBUFFERDATAPROC F;
    if(!F)
        F = ( PFNGLNAMEDBUFFERDATAPROC ) mygetprocaddr("glNamedBufferData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        params.push_back( (uint64_t) usage_ );
        for(auto& it : wildcardHooks )
            it("glNamedBufferData",params);
    }
    F(buffer_, size_, data_, usage_);
    return;
}
void glNamedBufferStorage ( GLuint buffer_, GLsizeiptr size_, const void * data_, GLbitfield flags_ ){
    static PFNGLNAMEDBUFFERSTORAGEPROC F;
    if(!F)
        F = ( PFNGLNAMEDBUFFERSTORAGEPROC ) mygetprocaddr("glNamedBufferStorage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        params.push_back( (uint64_t) flags_ );
        for(auto& it : wildcardHooks )
            it("glNamedBufferStorage",params);
    }
    F(buffer_, size_, data_, flags_);
    return;
}
void glNamedBufferSubData ( GLuint buffer_, GLintptr offset_, GLsizeiptr size_, const void * data_ ){
    static PFNGLNAMEDBUFFERSUBDATAPROC F;
    if(!F)
        F = ( PFNGLNAMEDBUFFERSUBDATAPROC ) mygetprocaddr("glNamedBufferSubData");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glNamedBufferSubData",params);
    }
    F(buffer_, offset_, size_, data_);
    return;
}
void glNamedFramebufferDrawBuffer ( GLuint framebuffer_, GLenum buf_ ){
    static PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC ) mygetprocaddr("glNamedFramebufferDrawBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) buf_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferDrawBuffer",params);
    }
    F(framebuffer_, buf_);
    return;
}
void glNamedFramebufferDrawBuffers ( GLuint framebuffer_, GLsizei n_, const GLenum * bufs_ ){
    static PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC ) mygetprocaddr("glNamedFramebufferDrawBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) n_ );
        params.push_back( (uint64_t) bufs_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferDrawBuffers",params);
    }
    F(framebuffer_, n_, bufs_);
    return;
}
void glNamedFramebufferParameteri ( GLuint framebuffer_, GLenum pname_, GLint param_ ){
    static PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC ) mygetprocaddr("glNamedFramebufferParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferParameteri",params);
    }
    F(framebuffer_, pname_, param_);
    return;
}
void glNamedFramebufferReadBuffer ( GLuint framebuffer_, GLenum src_ ){
    static PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC ) mygetprocaddr("glNamedFramebufferReadBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) src_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferReadBuffer",params);
    }
    F(framebuffer_, src_);
    return;
}
void glNamedFramebufferRenderbuffer ( GLuint framebuffer_, GLenum attachment_, GLenum renderbuffertarget_, GLuint renderbuffer_ ){
    static PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC ) mygetprocaddr("glNamedFramebufferRenderbuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) renderbuffertarget_ );
        params.push_back( (uint64_t) renderbuffer_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferRenderbuffer",params);
    }
    F(framebuffer_, attachment_, renderbuffertarget_, renderbuffer_);
    return;
}
void glNamedFramebufferTexture ( GLuint framebuffer_, GLenum attachment_, GLuint texture_, GLint level_ ){
    static PFNGLNAMEDFRAMEBUFFERTEXTUREPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERTEXTUREPROC ) mygetprocaddr("glNamedFramebufferTexture");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferTexture",params);
    }
    F(framebuffer_, attachment_, texture_, level_);
    return;
}
void glNamedFramebufferTextureLayer ( GLuint framebuffer_, GLenum attachment_, GLuint texture_, GLint level_, GLint layer_ ){
    static PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC F;
    if(!F)
        F = ( PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC ) mygetprocaddr("glNamedFramebufferTextureLayer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) framebuffer_ );
        params.push_back( (uint64_t) attachment_ );
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) layer_ );
        for(auto& it : wildcardHooks )
            it("glNamedFramebufferTextureLayer",params);
    }
    F(framebuffer_, attachment_, texture_, level_, layer_);
    return;
}
void glNamedRenderbufferStorage ( GLuint renderbuffer_, GLenum internalformat_, GLsizei width_, GLsizei height_ ){
    static PFNGLNAMEDRENDERBUFFERSTORAGEPROC F;
    if(!F)
        F = ( PFNGLNAMEDRENDERBUFFERSTORAGEPROC ) mygetprocaddr("glNamedRenderbufferStorage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) renderbuffer_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glNamedRenderbufferStorage",params);
    }
    F(renderbuffer_, internalformat_, width_, height_);
    return;
}
void glNamedRenderbufferStorageMultisample ( GLuint renderbuffer_, GLsizei samples_, GLenum internalformat_, GLsizei width_, GLsizei height_ ){
    static PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC F;
    if(!F)
        F = ( PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC ) mygetprocaddr("glNamedRenderbufferStorageMultisample");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) renderbuffer_ );
        params.push_back( (uint64_t) samples_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glNamedRenderbufferStorageMultisample",params);
    }
    F(renderbuffer_, samples_, internalformat_, width_, height_);
    return;
}
void glObjectLabel ( GLenum identifier_, GLuint name_, GLsizei length_, const GLchar * label_ ){
    static PFNGLOBJECTLABELPROC F;
    if(!F)
        F = ( PFNGLOBJECTLABELPROC ) mygetprocaddr("glObjectLabel");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) identifier_ );
        params.push_back( (uint64_t) name_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) label_ );
        for(auto& it : wildcardHooks )
            it("glObjectLabel",params);
    }
    F(identifier_, name_, length_, label_);
    return;
}
void glObjectPtrLabel ( const void * ptr_, GLsizei length_, const GLchar * label_ ){
    static PFNGLOBJECTPTRLABELPROC F;
    if(!F)
        F = ( PFNGLOBJECTPTRLABELPROC ) mygetprocaddr("glObjectPtrLabel");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) ptr_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) label_ );
        for(auto& it : wildcardHooks )
            it("glObjectPtrLabel",params);
    }
    F(ptr_, length_, label_);
    return;
}
void glPatchParameterfv ( GLenum pname_, const GLfloat * values_ ){
    static PFNGLPATCHPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLPATCHPARAMETERFVPROC ) mygetprocaddr("glPatchParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) values_ );
        for(auto& it : wildcardHooks )
            it("glPatchParameterfv",params);
    }
    F(pname_, values_);
    return;
}
void glPatchParameteri ( GLenum pname_, GLint value_ ){
    static PFNGLPATCHPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLPATCHPARAMETERIPROC ) mygetprocaddr("glPatchParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glPatchParameteri",params);
    }
    F(pname_, value_);
    return;
}
void glPauseTransformFeedback (  ){
    static PFNGLPAUSETRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLPAUSETRANSFORMFEEDBACKPROC ) mygetprocaddr("glPauseTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glPauseTransformFeedback",params);
    }
    F();
    return;
}
void glPixelStoref ( GLenum pname_, GLfloat param_ ){
    static PFNGLPIXELSTOREFPROC F;
    if(!F)
        F = ( PFNGLPIXELSTOREFPROC ) mygetprocaddr("glPixelStoref");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glPixelStoref",params);
    }
    F(pname_, param_);
    return;
}
void glPixelStorei ( GLenum pname_, GLint param_ ){
    static PFNGLPIXELSTOREIPROC F;
    if(!F)
        F = ( PFNGLPIXELSTOREIPROC ) mygetprocaddr("glPixelStorei");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glPixelStorei",params);
    }
    F(pname_, param_);
    return;
}
void glPointParameterf ( GLenum pname_, GLfloat param_ ){
    static PFNGLPOINTPARAMETERFPROC F;
    if(!F)
        F = ( PFNGLPOINTPARAMETERFPROC ) mygetprocaddr("glPointParameterf");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glPointParameterf",params);
    }
    F(pname_, param_);
    return;
}
void glPointParameterfv ( GLenum pname_, const GLfloat * params_ ){
    static PFNGLPOINTPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLPOINTPARAMETERFVPROC ) mygetprocaddr("glPointParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glPointParameterfv",params);
    }
    F(pname_, params_);
    return;
}
void glPointParameteri ( GLenum pname_, GLint param_ ){
    static PFNGLPOINTPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLPOINTPARAMETERIPROC ) mygetprocaddr("glPointParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glPointParameteri",params);
    }
    F(pname_, param_);
    return;
}
void glPointParameteriv ( GLenum pname_, const GLint * params_ ){
    static PFNGLPOINTPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLPOINTPARAMETERIVPROC ) mygetprocaddr("glPointParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glPointParameteriv",params);
    }
    F(pname_, params_);
    return;
}
void glPointSize ( GLfloat size_ ){
    static PFNGLPOINTSIZEPROC F;
    if(!F)
        F = ( PFNGLPOINTSIZEPROC ) mygetprocaddr("glPointSize");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glPointSize",params);
    }
    F(size_);
    return;
}
void glPolygonMode ( GLenum face_, GLenum mode_ ){
    static PFNGLPOLYGONMODEPROC F;
    if(!F)
        F = ( PFNGLPOLYGONMODEPROC ) mygetprocaddr("glPolygonMode");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) face_ );
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glPolygonMode",params);
    }
    F(face_, mode_);
    return;
}
void glPolygonOffset ( GLfloat factor_, GLfloat units_ ){
    static PFNGLPOLYGONOFFSETPROC F;
    if(!F)
        F = ( PFNGLPOLYGONOFFSETPROC ) mygetprocaddr("glPolygonOffset");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) factor_ );
        params.push_back( (uint64_t) units_ );
        for(auto& it : wildcardHooks )
            it("glPolygonOffset",params);
    }
    F(factor_, units_);
    return;
}
void glPopDebugGroup (  ){
    static PFNGLPOPDEBUGGROUPPROC F;
    if(!F)
        F = ( PFNGLPOPDEBUGGROUPPROC ) mygetprocaddr("glPopDebugGroup");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glPopDebugGroup",params);
    }
    F();
    return;
}
void glPrimitiveRestartIndex ( GLuint index_ ){
    static PFNGLPRIMITIVERESTARTINDEXPROC F;
    if(!F)
        F = ( PFNGLPRIMITIVERESTARTINDEXPROC ) mygetprocaddr("glPrimitiveRestartIndex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        for(auto& it : wildcardHooks )
            it("glPrimitiveRestartIndex",params);
    }
    F(index_);
    return;
}
void glProgramBinary ( GLuint program_, GLenum binaryFormat_, const void * binary_, GLsizei length_ ){
    static PFNGLPROGRAMBINARYPROC F;
    if(!F)
        F = ( PFNGLPROGRAMBINARYPROC ) mygetprocaddr("glProgramBinary");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) binaryFormat_ );
        params.push_back( (uint64_t) binary_ );
        params.push_back( (uint64_t) length_ );
        for(auto& it : wildcardHooks )
            it("glProgramBinary",params);
    }
    F(program_, binaryFormat_, binary_, length_);
    return;
}
void glProgramParameteri ( GLuint program_, GLenum pname_, GLint value_ ){
    static PFNGLPROGRAMPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLPROGRAMPARAMETERIPROC ) mygetprocaddr("glProgramParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramParameteri",params);
    }
    F(program_, pname_, value_);
    return;
}
void glProgramUniform1d ( GLuint program_, GLint location_, GLdouble v0_ ){
    static PFNGLPROGRAMUNIFORM1DPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1DPROC ) mygetprocaddr("glProgramUniform1d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1d",params);
    }
    F(program_, location_, v0_);
    return;
}
void glProgramUniform1dv ( GLuint program_, GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORM1DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1DVPROC ) mygetprocaddr("glProgramUniform1dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1dv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform1f ( GLuint program_, GLint location_, GLfloat v0_ ){
    static PFNGLPROGRAMUNIFORM1FPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1FPROC ) mygetprocaddr("glProgramUniform1f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1f",params);
    }
    F(program_, location_, v0_);
    return;
}
void glProgramUniform1fv ( GLuint program_, GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORM1FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1FVPROC ) mygetprocaddr("glProgramUniform1fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1fv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform1i ( GLuint program_, GLint location_, GLint v0_ ){
    static PFNGLPROGRAMUNIFORM1IPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1IPROC ) mygetprocaddr("glProgramUniform1i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1i",params);
    }
    F(program_, location_, v0_);
    return;
}
void glProgramUniform1iv ( GLuint program_, GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLPROGRAMUNIFORM1IVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1IVPROC ) mygetprocaddr("glProgramUniform1iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1iv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform1ui ( GLuint program_, GLint location_, GLuint v0_ ){
    static PFNGLPROGRAMUNIFORM1UIPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1UIPROC ) mygetprocaddr("glProgramUniform1ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1ui",params);
    }
    F(program_, location_, v0_);
    return;
}
void glProgramUniform1uiv ( GLuint program_, GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLPROGRAMUNIFORM1UIVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM1UIVPROC ) mygetprocaddr("glProgramUniform1uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform1uiv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform2d ( GLuint program_, GLint location_, GLdouble v0_, GLdouble v1_ ){
    static PFNGLPROGRAMUNIFORM2DPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2DPROC ) mygetprocaddr("glProgramUniform2d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2d",params);
    }
    F(program_, location_, v0_, v1_);
    return;
}
void glProgramUniform2dv ( GLuint program_, GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORM2DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2DVPROC ) mygetprocaddr("glProgramUniform2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2dv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform2f ( GLuint program_, GLint location_, GLfloat v0_, GLfloat v1_ ){
    static PFNGLPROGRAMUNIFORM2FPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2FPROC ) mygetprocaddr("glProgramUniform2f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2f",params);
    }
    F(program_, location_, v0_, v1_);
    return;
}
void glProgramUniform2fv ( GLuint program_, GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORM2FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2FVPROC ) mygetprocaddr("glProgramUniform2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2fv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform2i ( GLuint program_, GLint location_, GLint v0_, GLint v1_ ){
    static PFNGLPROGRAMUNIFORM2IPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2IPROC ) mygetprocaddr("glProgramUniform2i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2i",params);
    }
    F(program_, location_, v0_, v1_);
    return;
}
void glProgramUniform2iv ( GLuint program_, GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLPROGRAMUNIFORM2IVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2IVPROC ) mygetprocaddr("glProgramUniform2iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2iv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform2ui ( GLuint program_, GLint location_, GLuint v0_, GLuint v1_ ){
    static PFNGLPROGRAMUNIFORM2UIPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2UIPROC ) mygetprocaddr("glProgramUniform2ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2ui",params);
    }
    F(program_, location_, v0_, v1_);
    return;
}
void glProgramUniform2uiv ( GLuint program_, GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLPROGRAMUNIFORM2UIVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM2UIVPROC ) mygetprocaddr("glProgramUniform2uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform2uiv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform3d ( GLuint program_, GLint location_, GLdouble v0_, GLdouble v1_, GLdouble v2_ ){
    static PFNGLPROGRAMUNIFORM3DPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3DPROC ) mygetprocaddr("glProgramUniform3d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3d",params);
    }
    F(program_, location_, v0_, v1_, v2_);
    return;
}
void glProgramUniform3dv ( GLuint program_, GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORM3DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3DVPROC ) mygetprocaddr("glProgramUniform3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3dv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform3f ( GLuint program_, GLint location_, GLfloat v0_, GLfloat v1_, GLfloat v2_ ){
    static PFNGLPROGRAMUNIFORM3FPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3FPROC ) mygetprocaddr("glProgramUniform3f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3f",params);
    }
    F(program_, location_, v0_, v1_, v2_);
    return;
}
void glProgramUniform3fv ( GLuint program_, GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORM3FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3FVPROC ) mygetprocaddr("glProgramUniform3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3fv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform3i ( GLuint program_, GLint location_, GLint v0_, GLint v1_, GLint v2_ ){
    static PFNGLPROGRAMUNIFORM3IPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3IPROC ) mygetprocaddr("glProgramUniform3i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3i",params);
    }
    F(program_, location_, v0_, v1_, v2_);
    return;
}
void glProgramUniform3iv ( GLuint program_, GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLPROGRAMUNIFORM3IVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3IVPROC ) mygetprocaddr("glProgramUniform3iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3iv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform3ui ( GLuint program_, GLint location_, GLuint v0_, GLuint v1_, GLuint v2_ ){
    static PFNGLPROGRAMUNIFORM3UIPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3UIPROC ) mygetprocaddr("glProgramUniform3ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3ui",params);
    }
    F(program_, location_, v0_, v1_, v2_);
    return;
}
void glProgramUniform3uiv ( GLuint program_, GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLPROGRAMUNIFORM3UIVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM3UIVPROC ) mygetprocaddr("glProgramUniform3uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform3uiv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform4d ( GLuint program_, GLint location_, GLdouble v0_, GLdouble v1_, GLdouble v2_, GLdouble v3_ ){
    static PFNGLPROGRAMUNIFORM4DPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4DPROC ) mygetprocaddr("glProgramUniform4d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4d",params);
    }
    F(program_, location_, v0_, v1_, v2_, v3_);
    return;
}
void glProgramUniform4dv ( GLuint program_, GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORM4DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4DVPROC ) mygetprocaddr("glProgramUniform4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4dv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform4f ( GLuint program_, GLint location_, GLfloat v0_, GLfloat v1_, GLfloat v2_, GLfloat v3_ ){
    static PFNGLPROGRAMUNIFORM4FPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4FPROC ) mygetprocaddr("glProgramUniform4f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4f",params);
    }
    F(program_, location_, v0_, v1_, v2_, v3_);
    return;
}
void glProgramUniform4fv ( GLuint program_, GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORM4FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4FVPROC ) mygetprocaddr("glProgramUniform4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4fv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform4i ( GLuint program_, GLint location_, GLint v0_, GLint v1_, GLint v2_, GLint v3_ ){
    static PFNGLPROGRAMUNIFORM4IPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4IPROC ) mygetprocaddr("glProgramUniform4i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4i",params);
    }
    F(program_, location_, v0_, v1_, v2_, v3_);
    return;
}
void glProgramUniform4iv ( GLuint program_, GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLPROGRAMUNIFORM4IVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4IVPROC ) mygetprocaddr("glProgramUniform4iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4iv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniform4ui ( GLuint program_, GLint location_, GLuint v0_, GLuint v1_, GLuint v2_, GLuint v3_ ){
    static PFNGLPROGRAMUNIFORM4UIPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4UIPROC ) mygetprocaddr("glProgramUniform4ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4ui",params);
    }
    F(program_, location_, v0_, v1_, v2_, v3_);
    return;
}
void glProgramUniform4uiv ( GLuint program_, GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLPROGRAMUNIFORM4UIVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORM4UIVPROC ) mygetprocaddr("glProgramUniform4uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniform4uiv",params);
    }
    F(program_, location_, count_, value_);
    return;
}
void glProgramUniformMatrix2dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX2DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX2DVPROC ) mygetprocaddr("glProgramUniformMatrix2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix2dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix2fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX2FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX2FVPROC ) mygetprocaddr("glProgramUniformMatrix2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix2fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix2x3dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC ) mygetprocaddr("glProgramUniformMatrix2x3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix2x3dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix2x3fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC ) mygetprocaddr("glProgramUniformMatrix2x3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix2x3fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix2x4dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC ) mygetprocaddr("glProgramUniformMatrix2x4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix2x4dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix2x4fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC ) mygetprocaddr("glProgramUniformMatrix2x4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix2x4fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix3dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX3DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX3DVPROC ) mygetprocaddr("glProgramUniformMatrix3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix3dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix3fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX3FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX3FVPROC ) mygetprocaddr("glProgramUniformMatrix3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix3fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix3x2dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC ) mygetprocaddr("glProgramUniformMatrix3x2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix3x2dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix3x2fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC ) mygetprocaddr("glProgramUniformMatrix3x2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix3x2fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix3x4dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC ) mygetprocaddr("glProgramUniformMatrix3x4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix3x4dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix3x4fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC ) mygetprocaddr("glProgramUniformMatrix3x4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix3x4fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix4dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX4DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX4DVPROC ) mygetprocaddr("glProgramUniformMatrix4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix4dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix4fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX4FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX4FVPROC ) mygetprocaddr("glProgramUniformMatrix4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix4fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix4x2dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC ) mygetprocaddr("glProgramUniformMatrix4x2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix4x2dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix4x2fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC ) mygetprocaddr("glProgramUniformMatrix4x2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix4x2fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix4x3dv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC ) mygetprocaddr("glProgramUniformMatrix4x3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix4x3dv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProgramUniformMatrix4x3fv ( GLuint program_, GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC F;
    if(!F)
        F = ( PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC ) mygetprocaddr("glProgramUniformMatrix4x3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glProgramUniformMatrix4x3fv",params);
    }
    F(program_, location_, count_, transpose_, value_);
    return;
}
void glProvokingVertex ( GLenum mode_ ){
    static PFNGLPROVOKINGVERTEXPROC F;
    if(!F)
        F = ( PFNGLPROVOKINGVERTEXPROC ) mygetprocaddr("glProvokingVertex");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mode_ );
        for(auto& it : wildcardHooks )
            it("glProvokingVertex",params);
    }
    F(mode_);
    return;
}
void glPushDebugGroup ( GLenum source_, GLuint id_, GLsizei length_, const GLchar * message_ ){
    static PFNGLPUSHDEBUGGROUPPROC F;
    if(!F)
        F = ( PFNGLPUSHDEBUGGROUPPROC ) mygetprocaddr("glPushDebugGroup");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) source_ );
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) length_ );
        params.push_back( (uint64_t) message_ );
        for(auto& it : wildcardHooks )
            it("glPushDebugGroup",params);
    }
    F(source_, id_, length_, message_);
    return;
}
void glQueryCounter ( GLuint id_, GLenum target_ ){
    static PFNGLQUERYCOUNTERPROC F;
    if(!F)
        F = ( PFNGLQUERYCOUNTERPROC ) mygetprocaddr("glQueryCounter");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) id_ );
        params.push_back( (uint64_t) target_ );
        for(auto& it : wildcardHooks )
            it("glQueryCounter",params);
    }
    F(id_, target_);
    return;
}
void glReadBuffer ( GLenum src_ ){
    static PFNGLREADBUFFERPROC F;
    if(!F)
        F = ( PFNGLREADBUFFERPROC ) mygetprocaddr("glReadBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) src_ );
        for(auto& it : wildcardHooks )
            it("glReadBuffer",params);
    }
    F(src_);
    return;
}
void glReadPixels ( GLint x_, GLint y_, GLsizei width_, GLsizei height_, GLenum format_, GLenum type_, void * pixels_ ){
    static PFNGLREADPIXELSPROC F;
    if(!F)
        F = ( PFNGLREADPIXELSPROC ) mygetprocaddr("glReadPixels");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glReadPixels",params);
    }
    F(x_, y_, width_, height_, format_, type_, pixels_);
    return;
}
void glReadnPixels ( GLint x_, GLint y_, GLsizei width_, GLsizei height_, GLenum format_, GLenum type_, GLsizei bufSize_, void * data_ ){
    static PFNGLREADNPIXELSPROC F;
    if(!F)
        F = ( PFNGLREADNPIXELSPROC ) mygetprocaddr("glReadnPixels");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) bufSize_ );
        params.push_back( (uint64_t) data_ );
        for(auto& it : wildcardHooks )
            it("glReadnPixels",params);
    }
    F(x_, y_, width_, height_, format_, type_, bufSize_, data_);
    return;
}
void glReleaseShaderCompiler (  ){
    static PFNGLRELEASESHADERCOMPILERPROC F;
    if(!F)
        F = ( PFNGLRELEASESHADERCOMPILERPROC ) mygetprocaddr("glReleaseShaderCompiler");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glReleaseShaderCompiler",params);
    }
    F();
    return;
}
void glRenderbufferStorage ( GLenum target_, GLenum internalformat_, GLsizei width_, GLsizei height_ ){
    static PFNGLRENDERBUFFERSTORAGEPROC F;
    if(!F)
        F = ( PFNGLRENDERBUFFERSTORAGEPROC ) mygetprocaddr("glRenderbufferStorage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glRenderbufferStorage",params);
    }
    F(target_, internalformat_, width_, height_);
    return;
}
void glRenderbufferStorageMultisample ( GLenum target_, GLsizei samples_, GLenum internalformat_, GLsizei width_, GLsizei height_ ){
    static PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC F;
    if(!F)
        F = ( PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC ) mygetprocaddr("glRenderbufferStorageMultisample");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) samples_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glRenderbufferStorageMultisample",params);
    }
    F(target_, samples_, internalformat_, width_, height_);
    return;
}
void glResumeTransformFeedback (  ){
    static PFNGLRESUMETRANSFORMFEEDBACKPROC F;
    if(!F)
        F = ( PFNGLRESUMETRANSFORMFEEDBACKPROC ) mygetprocaddr("glResumeTransformFeedback");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glResumeTransformFeedback",params);
    }
    F();
    return;
}
void glSampleCoverage ( GLfloat value_, GLboolean invert_ ){
    static PFNGLSAMPLECOVERAGEPROC F;
    if(!F)
        F = ( PFNGLSAMPLECOVERAGEPROC ) mygetprocaddr("glSampleCoverage");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) value_ );
        params.push_back( (uint64_t) invert_ );
        for(auto& it : wildcardHooks )
            it("glSampleCoverage",params);
    }
    F(value_, invert_);
    return;
}
void glSampleMaski ( GLuint maskNumber_, GLbitfield mask_ ){
    static PFNGLSAMPLEMASKIPROC F;
    if(!F)
        F = ( PFNGLSAMPLEMASKIPROC ) mygetprocaddr("glSampleMaski");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) maskNumber_ );
        params.push_back( (uint64_t) mask_ );
        for(auto& it : wildcardHooks )
            it("glSampleMaski",params);
    }
    F(maskNumber_, mask_);
    return;
}
void glSamplerParameterIiv ( GLuint sampler_, GLenum pname_, const GLint * param_ ){
    static PFNGLSAMPLERPARAMETERIIVPROC F;
    if(!F)
        F = ( PFNGLSAMPLERPARAMETERIIVPROC ) mygetprocaddr("glSamplerParameterIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glSamplerParameterIiv",params);
    }
    F(sampler_, pname_, param_);
    return;
}
void glSamplerParameterIuiv ( GLuint sampler_, GLenum pname_, const GLuint * param_ ){
    static PFNGLSAMPLERPARAMETERIUIVPROC F;
    if(!F)
        F = ( PFNGLSAMPLERPARAMETERIUIVPROC ) mygetprocaddr("glSamplerParameterIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glSamplerParameterIuiv",params);
    }
    F(sampler_, pname_, param_);
    return;
}
void glSamplerParameterf ( GLuint sampler_, GLenum pname_, GLfloat param_ ){
    static PFNGLSAMPLERPARAMETERFPROC F;
    if(!F)
        F = ( PFNGLSAMPLERPARAMETERFPROC ) mygetprocaddr("glSamplerParameterf");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glSamplerParameterf",params);
    }
    F(sampler_, pname_, param_);
    return;
}
void glSamplerParameterfv ( GLuint sampler_, GLenum pname_, const GLfloat * param_ ){
    static PFNGLSAMPLERPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLSAMPLERPARAMETERFVPROC ) mygetprocaddr("glSamplerParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glSamplerParameterfv",params);
    }
    F(sampler_, pname_, param_);
    return;
}
void glSamplerParameteri ( GLuint sampler_, GLenum pname_, GLint param_ ){
    static PFNGLSAMPLERPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLSAMPLERPARAMETERIPROC ) mygetprocaddr("glSamplerParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glSamplerParameteri",params);
    }
    F(sampler_, pname_, param_);
    return;
}
void glSamplerParameteriv ( GLuint sampler_, GLenum pname_, const GLint * param_ ){
    static PFNGLSAMPLERPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLSAMPLERPARAMETERIVPROC ) mygetprocaddr("glSamplerParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sampler_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glSamplerParameteriv",params);
    }
    F(sampler_, pname_, param_);
    return;
}
void glScissor ( GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLSCISSORPROC F;
    if(!F)
        F = ( PFNGLSCISSORPROC ) mygetprocaddr("glScissor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glScissor",params);
    }
    F(x_, y_, width_, height_);
    return;
}
void glScissorArrayv ( GLuint first_, GLsizei count_, const GLint * v_ ){
    static PFNGLSCISSORARRAYVPROC F;
    if(!F)
        F = ( PFNGLSCISSORARRAYVPROC ) mygetprocaddr("glScissorArrayv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glScissorArrayv",params);
    }
    F(first_, count_, v_);
    return;
}
void glScissorIndexed ( GLuint index_, GLint left_, GLint bottom_, GLsizei width_, GLsizei height_ ){
    static PFNGLSCISSORINDEXEDPROC F;
    if(!F)
        F = ( PFNGLSCISSORINDEXEDPROC ) mygetprocaddr("glScissorIndexed");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) left_ );
        params.push_back( (uint64_t) bottom_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glScissorIndexed",params);
    }
    F(index_, left_, bottom_, width_, height_);
    return;
}
void glScissorIndexedv ( GLuint index_, const GLint * v_ ){
    static PFNGLSCISSORINDEXEDVPROC F;
    if(!F)
        F = ( PFNGLSCISSORINDEXEDVPROC ) mygetprocaddr("glScissorIndexedv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glScissorIndexedv",params);
    }
    F(index_, v_);
    return;
}
void glShaderBinary ( GLsizei count_, const GLuint * shaders_, GLenum binaryformat_, const void * binary_, GLsizei length_ ){
    static PFNGLSHADERBINARYPROC F;
    if(!F)
        F = ( PFNGLSHADERBINARYPROC ) mygetprocaddr("glShaderBinary");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) shaders_ );
        params.push_back( (uint64_t) binaryformat_ );
        params.push_back( (uint64_t) binary_ );
        params.push_back( (uint64_t) length_ );
        for(auto& it : wildcardHooks )
            it("glShaderBinary",params);
    }
    F(count_, shaders_, binaryformat_, binary_, length_);
    return;
}
void glShaderSource ( GLuint shader_, GLsizei count_, const GLchar ** string_, const GLint * length_ ){
    static PFNGLSHADERSOURCEPROC F;
    if(!F)
        F = ( PFNGLSHADERSOURCEPROC ) mygetprocaddr("glShaderSource");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shader_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) string_ );
        params.push_back( (uint64_t) length_ );
        for(auto& it : wildcardHooks )
            it("glShaderSource",params);
    }
    F(shader_, count_, string_, length_);
    return;
}
void glShaderStorageBlockBinding ( GLuint program_, GLuint storageBlockIndex_, GLuint storageBlockBinding_ ){
    static PFNGLSHADERSTORAGEBLOCKBINDINGPROC F;
    if(!F)
        F = ( PFNGLSHADERSTORAGEBLOCKBINDINGPROC ) mygetprocaddr("glShaderStorageBlockBinding");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) storageBlockIndex_ );
        params.push_back( (uint64_t) storageBlockBinding_ );
        for(auto& it : wildcardHooks )
            it("glShaderStorageBlockBinding",params);
    }
    F(program_, storageBlockIndex_, storageBlockBinding_);
    return;
}
void glStencilFunc ( GLenum func_, GLint ref_, GLuint mask_ ){
    static PFNGLSTENCILFUNCPROC F;
    if(!F)
        F = ( PFNGLSTENCILFUNCPROC ) mygetprocaddr("glStencilFunc");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) func_ );
        params.push_back( (uint64_t) ref_ );
        params.push_back( (uint64_t) mask_ );
        for(auto& it : wildcardHooks )
            it("glStencilFunc",params);
    }
    F(func_, ref_, mask_);
    return;
}
void glStencilFuncSeparate ( GLenum face_, GLenum func_, GLint ref_, GLuint mask_ ){
    static PFNGLSTENCILFUNCSEPARATEPROC F;
    if(!F)
        F = ( PFNGLSTENCILFUNCSEPARATEPROC ) mygetprocaddr("glStencilFuncSeparate");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) face_ );
        params.push_back( (uint64_t) func_ );
        params.push_back( (uint64_t) ref_ );
        params.push_back( (uint64_t) mask_ );
        for(auto& it : wildcardHooks )
            it("glStencilFuncSeparate",params);
    }
    F(face_, func_, ref_, mask_);
    return;
}
void glStencilMask ( GLuint mask_ ){
    static PFNGLSTENCILMASKPROC F;
    if(!F)
        F = ( PFNGLSTENCILMASKPROC ) mygetprocaddr("glStencilMask");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) mask_ );
        for(auto& it : wildcardHooks )
            it("glStencilMask",params);
    }
    F(mask_);
    return;
}
void glStencilMaskSeparate ( GLenum face_, GLuint mask_ ){
    static PFNGLSTENCILMASKSEPARATEPROC F;
    if(!F)
        F = ( PFNGLSTENCILMASKSEPARATEPROC ) mygetprocaddr("glStencilMaskSeparate");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) face_ );
        params.push_back( (uint64_t) mask_ );
        for(auto& it : wildcardHooks )
            it("glStencilMaskSeparate",params);
    }
    F(face_, mask_);
    return;
}
void glStencilOp ( GLenum fail_, GLenum zfail_, GLenum zpass_ ){
    static PFNGLSTENCILOPPROC F;
    if(!F)
        F = ( PFNGLSTENCILOPPROC ) mygetprocaddr("glStencilOp");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) fail_ );
        params.push_back( (uint64_t) zfail_ );
        params.push_back( (uint64_t) zpass_ );
        for(auto& it : wildcardHooks )
            it("glStencilOp",params);
    }
    F(fail_, zfail_, zpass_);
    return;
}
void glStencilOpSeparate ( GLenum face_, GLenum sfail_, GLenum dpfail_, GLenum dppass_ ){
    static PFNGLSTENCILOPSEPARATEPROC F;
    if(!F)
        F = ( PFNGLSTENCILOPSEPARATEPROC ) mygetprocaddr("glStencilOpSeparate");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) face_ );
        params.push_back( (uint64_t) sfail_ );
        params.push_back( (uint64_t) dpfail_ );
        params.push_back( (uint64_t) dppass_ );
        for(auto& it : wildcardHooks )
            it("glStencilOpSeparate",params);
    }
    F(face_, sfail_, dpfail_, dppass_);
    return;
}
void glTexBuffer ( GLenum target_, GLenum internalformat_, GLuint buffer_ ){
    static PFNGLTEXBUFFERPROC F;
    if(!F)
        F = ( PFNGLTEXBUFFERPROC ) mygetprocaddr("glTexBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glTexBuffer",params);
    }
    F(target_, internalformat_, buffer_);
    return;
}
void glTexBufferRange ( GLenum target_, GLenum internalformat_, GLuint buffer_, GLintptr offset_, GLsizeiptr size_ ){
    static PFNGLTEXBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLTEXBUFFERRANGEPROC ) mygetprocaddr("glTexBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glTexBufferRange",params);
    }
    F(target_, internalformat_, buffer_, offset_, size_);
    return;
}
void glTexImage1D ( GLenum target_, GLint level_, GLint internalformat_, GLsizei width_, GLint border_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLTEXIMAGE1DPROC ) mygetprocaddr("glTexImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) border_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTexImage1D",params);
    }
    F(target_, level_, internalformat_, width_, border_, format_, type_, pixels_);
    return;
}
void glTexImage2D ( GLenum target_, GLint level_, GLint internalformat_, GLsizei width_, GLsizei height_, GLint border_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLTEXIMAGE2DPROC ) mygetprocaddr("glTexImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) border_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTexImage2D",params);
    }
    F(target_, level_, internalformat_, width_, height_, border_, format_, type_, pixels_);
    return;
}
void glTexImage3D ( GLenum target_, GLint level_, GLint internalformat_, GLsizei width_, GLsizei height_, GLsizei depth_, GLint border_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLTEXIMAGE3DPROC ) mygetprocaddr("glTexImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) border_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTexImage3D",params);
    }
    F(target_, level_, internalformat_, width_, height_, depth_, border_, format_, type_, pixels_);
    return;
}
void glTexParameterIiv ( GLenum target_, GLenum pname_, const GLint * params_ ){
    static PFNGLTEXPARAMETERIIVPROC F;
    if(!F)
        F = ( PFNGLTEXPARAMETERIIVPROC ) mygetprocaddr("glTexParameterIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glTexParameterIiv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glTexParameterIuiv ( GLenum target_, GLenum pname_, const GLuint * params_ ){
    static PFNGLTEXPARAMETERIUIVPROC F;
    if(!F)
        F = ( PFNGLTEXPARAMETERIUIVPROC ) mygetprocaddr("glTexParameterIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glTexParameterIuiv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glTexParameterf ( GLenum target_, GLenum pname_, GLfloat param_ ){
    static PFNGLTEXPARAMETERFPROC F;
    if(!F)
        F = ( PFNGLTEXPARAMETERFPROC ) mygetprocaddr("glTexParameterf");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glTexParameterf",params);
    }
    F(target_, pname_, param_);
    return;
}
void glTexParameterfv ( GLenum target_, GLenum pname_, const GLfloat * params_ ){
    static PFNGLTEXPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLTEXPARAMETERFVPROC ) mygetprocaddr("glTexParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glTexParameterfv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glTexParameteri ( GLenum target_, GLenum pname_, GLint param_ ){
    static PFNGLTEXPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLTEXPARAMETERIPROC ) mygetprocaddr("glTexParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glTexParameteri",params);
    }
    F(target_, pname_, param_);
    return;
}
void glTexParameteriv ( GLenum target_, GLenum pname_, const GLint * params_ ){
    static PFNGLTEXPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLTEXPARAMETERIVPROC ) mygetprocaddr("glTexParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glTexParameteriv",params);
    }
    F(target_, pname_, params_);
    return;
}
void glTexStorage1D ( GLenum target_, GLsizei levels_, GLenum internalformat_, GLsizei width_ ){
    static PFNGLTEXSTORAGE1DPROC F;
    if(!F)
        F = ( PFNGLTEXSTORAGE1DPROC ) mygetprocaddr("glTexStorage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) levels_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        for(auto& it : wildcardHooks )
            it("glTexStorage1D",params);
    }
    F(target_, levels_, internalformat_, width_);
    return;
}
void glTexStorage2D ( GLenum target_, GLsizei levels_, GLenum internalformat_, GLsizei width_, GLsizei height_ ){
    static PFNGLTEXSTORAGE2DPROC F;
    if(!F)
        F = ( PFNGLTEXSTORAGE2DPROC ) mygetprocaddr("glTexStorage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) levels_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glTexStorage2D",params);
    }
    F(target_, levels_, internalformat_, width_, height_);
    return;
}
void glTexStorage3D ( GLenum target_, GLsizei levels_, GLenum internalformat_, GLsizei width_, GLsizei height_, GLsizei depth_ ){
    static PFNGLTEXSTORAGE3DPROC F;
    if(!F)
        F = ( PFNGLTEXSTORAGE3DPROC ) mygetprocaddr("glTexStorage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) levels_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        for(auto& it : wildcardHooks )
            it("glTexStorage3D",params);
    }
    F(target_, levels_, internalformat_, width_, height_, depth_);
    return;
}
void glTexSubImage1D ( GLenum target_, GLint level_, GLint xoffset_, GLsizei width_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXSUBIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLTEXSUBIMAGE1DPROC ) mygetprocaddr("glTexSubImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTexSubImage1D",params);
    }
    F(target_, level_, xoffset_, width_, format_, type_, pixels_);
    return;
}
void glTexSubImage2D ( GLenum target_, GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXSUBIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLTEXSUBIMAGE2DPROC ) mygetprocaddr("glTexSubImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTexSubImage2D",params);
    }
    F(target_, level_, xoffset_, yoffset_, width_, height_, format_, type_, pixels_);
    return;
}
void glTexSubImage3D ( GLenum target_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXSUBIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLTEXSUBIMAGE3DPROC ) mygetprocaddr("glTexSubImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTexSubImage3D",params);
    }
    F(target_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, format_, type_, pixels_);
    return;
}
void glTextureBarrier (  ){
    static PFNGLTEXTUREBARRIERPROC F;
    if(!F)
        F = ( PFNGLTEXTUREBARRIERPROC ) mygetprocaddr("glTextureBarrier");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        for(auto& it : wildcardHooks )
            it("glTextureBarrier",params);
    }
    F();
    return;
}
void glTextureBuffer ( GLuint texture_, GLenum internalformat_, GLuint buffer_ ){
    static PFNGLTEXTUREBUFFERPROC F;
    if(!F)
        F = ( PFNGLTEXTUREBUFFERPROC ) mygetprocaddr("glTextureBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glTextureBuffer",params);
    }
    F(texture_, internalformat_, buffer_);
    return;
}
void glTextureBufferRange ( GLuint texture_, GLenum internalformat_, GLuint buffer_, GLintptr offset_, GLsizeiptr size_ ){
    static PFNGLTEXTUREBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLTEXTUREBUFFERRANGEPROC ) mygetprocaddr("glTextureBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glTextureBufferRange",params);
    }
    F(texture_, internalformat_, buffer_, offset_, size_);
    return;
}
void glTextureParameterIiv ( GLuint texture_, GLenum pname_, const GLint * params_ ){
    static PFNGLTEXTUREPARAMETERIIVPROC F;
    if(!F)
        F = ( PFNGLTEXTUREPARAMETERIIVPROC ) mygetprocaddr("glTextureParameterIiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glTextureParameterIiv",params);
    }
    F(texture_, pname_, params_);
    return;
}
void glTextureParameterIuiv ( GLuint texture_, GLenum pname_, const GLuint * params_ ){
    static PFNGLTEXTUREPARAMETERIUIVPROC F;
    if(!F)
        F = ( PFNGLTEXTUREPARAMETERIUIVPROC ) mygetprocaddr("glTextureParameterIuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) params_ );
        for(auto& it : wildcardHooks )
            it("glTextureParameterIuiv",params);
    }
    F(texture_, pname_, params_);
    return;
}
void glTextureParameterf ( GLuint texture_, GLenum pname_, GLfloat param_ ){
    static PFNGLTEXTUREPARAMETERFPROC F;
    if(!F)
        F = ( PFNGLTEXTUREPARAMETERFPROC ) mygetprocaddr("glTextureParameterf");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glTextureParameterf",params);
    }
    F(texture_, pname_, param_);
    return;
}
void glTextureParameterfv ( GLuint texture_, GLenum pname_, const GLfloat * param_ ){
    static PFNGLTEXTUREPARAMETERFVPROC F;
    if(!F)
        F = ( PFNGLTEXTUREPARAMETERFVPROC ) mygetprocaddr("glTextureParameterfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glTextureParameterfv",params);
    }
    F(texture_, pname_, param_);
    return;
}
void glTextureParameteri ( GLuint texture_, GLenum pname_, GLint param_ ){
    static PFNGLTEXTUREPARAMETERIPROC F;
    if(!F)
        F = ( PFNGLTEXTUREPARAMETERIPROC ) mygetprocaddr("glTextureParameteri");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glTextureParameteri",params);
    }
    F(texture_, pname_, param_);
    return;
}
void glTextureParameteriv ( GLuint texture_, GLenum pname_, const GLint * param_ ){
    static PFNGLTEXTUREPARAMETERIVPROC F;
    if(!F)
        F = ( PFNGLTEXTUREPARAMETERIVPROC ) mygetprocaddr("glTextureParameteriv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) pname_ );
        params.push_back( (uint64_t) param_ );
        for(auto& it : wildcardHooks )
            it("glTextureParameteriv",params);
    }
    F(texture_, pname_, param_);
    return;
}
void glTextureStorage1D ( GLuint texture_, GLsizei levels_, GLenum internalformat_, GLsizei width_ ){
    static PFNGLTEXTURESTORAGE1DPROC F;
    if(!F)
        F = ( PFNGLTEXTURESTORAGE1DPROC ) mygetprocaddr("glTextureStorage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) levels_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        for(auto& it : wildcardHooks )
            it("glTextureStorage1D",params);
    }
    F(texture_, levels_, internalformat_, width_);
    return;
}
void glTextureStorage2D ( GLuint texture_, GLsizei levels_, GLenum internalformat_, GLsizei width_, GLsizei height_ ){
    static PFNGLTEXTURESTORAGE2DPROC F;
    if(!F)
        F = ( PFNGLTEXTURESTORAGE2DPROC ) mygetprocaddr("glTextureStorage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) levels_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glTextureStorage2D",params);
    }
    F(texture_, levels_, internalformat_, width_, height_);
    return;
}
void glTextureStorage3D ( GLuint texture_, GLsizei levels_, GLenum internalformat_, GLsizei width_, GLsizei height_, GLsizei depth_ ){
    static PFNGLTEXTURESTORAGE3DPROC F;
    if(!F)
        F = ( PFNGLTEXTURESTORAGE3DPROC ) mygetprocaddr("glTextureStorage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) levels_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        for(auto& it : wildcardHooks )
            it("glTextureStorage3D",params);
    }
    F(texture_, levels_, internalformat_, width_, height_, depth_);
    return;
}
void glTextureSubImage1D ( GLuint texture_, GLint level_, GLint xoffset_, GLsizei width_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXTURESUBIMAGE1DPROC F;
    if(!F)
        F = ( PFNGLTEXTURESUBIMAGE1DPROC ) mygetprocaddr("glTextureSubImage1D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTextureSubImage1D",params);
    }
    F(texture_, level_, xoffset_, width_, format_, type_, pixels_);
    return;
}
void glTextureSubImage2D ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXTURESUBIMAGE2DPROC F;
    if(!F)
        F = ( PFNGLTEXTURESUBIMAGE2DPROC ) mygetprocaddr("glTextureSubImage2D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTextureSubImage2D",params);
    }
    F(texture_, level_, xoffset_, yoffset_, width_, height_, format_, type_, pixels_);
    return;
}
void glTextureSubImage3D ( GLuint texture_, GLint level_, GLint xoffset_, GLint yoffset_, GLint zoffset_, GLsizei width_, GLsizei height_, GLsizei depth_, GLenum format_, GLenum type_, const void * pixels_ ){
    static PFNGLTEXTURESUBIMAGE3DPROC F;
    if(!F)
        F = ( PFNGLTEXTURESUBIMAGE3DPROC ) mygetprocaddr("glTextureSubImage3D");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) level_ );
        params.push_back( (uint64_t) xoffset_ );
        params.push_back( (uint64_t) yoffset_ );
        params.push_back( (uint64_t) zoffset_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        params.push_back( (uint64_t) depth_ );
        params.push_back( (uint64_t) format_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) pixels_ );
        for(auto& it : wildcardHooks )
            it("glTextureSubImage3D",params);
    }
    F(texture_, level_, xoffset_, yoffset_, zoffset_, width_, height_, depth_, format_, type_, pixels_);
    return;
}
void glTextureView ( GLuint texture_, GLenum target_, GLuint origtexture_, GLenum internalformat_, GLuint minlevel_, GLuint numlevels_, GLuint minlayer_, GLuint numlayers_ ){
    static PFNGLTEXTUREVIEWPROC F;
    if(!F)
        F = ( PFNGLTEXTUREVIEWPROC ) mygetprocaddr("glTextureView");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) texture_ );
        params.push_back( (uint64_t) target_ );
        params.push_back( (uint64_t) origtexture_ );
        params.push_back( (uint64_t) internalformat_ );
        params.push_back( (uint64_t) minlevel_ );
        params.push_back( (uint64_t) numlevels_ );
        params.push_back( (uint64_t) minlayer_ );
        params.push_back( (uint64_t) numlayers_ );
        for(auto& it : wildcardHooks )
            it("glTextureView",params);
    }
    F(texture_, target_, origtexture_, internalformat_, minlevel_, numlevels_, minlayer_, numlayers_);
    return;
}
void glTransformFeedbackBufferBase ( GLuint xfb_, GLuint index_, GLuint buffer_ ){
    static PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC F;
    if(!F)
        F = ( PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC ) mygetprocaddr("glTransformFeedbackBufferBase");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) xfb_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glTransformFeedbackBufferBase",params);
    }
    F(xfb_, index_, buffer_);
    return;
}
void glTransformFeedbackBufferRange ( GLuint xfb_, GLuint index_, GLuint buffer_, GLintptr offset_, GLsizeiptr size_ ){
    static PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC F;
    if(!F)
        F = ( PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC ) mygetprocaddr("glTransformFeedbackBufferRange");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) xfb_ );
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) size_ );
        for(auto& it : wildcardHooks )
            it("glTransformFeedbackBufferRange",params);
    }
    F(xfb_, index_, buffer_, offset_, size_);
    return;
}
void glTransformFeedbackVaryings ( GLuint program_, GLsizei count_, const GLchar ** varyings_, GLenum bufferMode_ ){
    static PFNGLTRANSFORMFEEDBACKVARYINGSPROC F;
    if(!F)
        F = ( PFNGLTRANSFORMFEEDBACKVARYINGSPROC ) mygetprocaddr("glTransformFeedbackVaryings");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) varyings_ );
        params.push_back( (uint64_t) bufferMode_ );
        for(auto& it : wildcardHooks )
            it("glTransformFeedbackVaryings",params);
    }
    F(program_, count_, varyings_, bufferMode_);
    return;
}
void glUniform1d ( GLint location_, GLdouble x_ ){
    static PFNGLUNIFORM1DPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1DPROC ) mygetprocaddr("glUniform1d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glUniform1d",params);
    }
    F(location_, x_);
    return;
}
void glUniform1dv ( GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLUNIFORM1DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1DVPROC ) mygetprocaddr("glUniform1dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform1dv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform1f ( GLint location_, GLfloat v0_ ){
    static PFNGLUNIFORM1FPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1FPROC ) mygetprocaddr("glUniform1f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glUniform1f",params);
    }
    F(location_, v0_);
    return;
}
void glUniform1fv ( GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLUNIFORM1FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1FVPROC ) mygetprocaddr("glUniform1fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform1fv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform1i ( GLint location_, GLint v0_ ){
    static PFNGLUNIFORM1IPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1IPROC ) mygetprocaddr("glUniform1i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glUniform1i",params);
    }
    F(location_, v0_);
    return;
}
void glUniform1iv ( GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLUNIFORM1IVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1IVPROC ) mygetprocaddr("glUniform1iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform1iv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform1ui ( GLint location_, GLuint v0_ ){
    static PFNGLUNIFORM1UIPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1UIPROC ) mygetprocaddr("glUniform1ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        for(auto& it : wildcardHooks )
            it("glUniform1ui",params);
    }
    F(location_, v0_);
    return;
}
void glUniform1uiv ( GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLUNIFORM1UIVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM1UIVPROC ) mygetprocaddr("glUniform1uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform1uiv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform2d ( GLint location_, GLdouble x_, GLdouble y_ ){
    static PFNGLUNIFORM2DPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2DPROC ) mygetprocaddr("glUniform2d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glUniform2d",params);
    }
    F(location_, x_, y_);
    return;
}
void glUniform2dv ( GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLUNIFORM2DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2DVPROC ) mygetprocaddr("glUniform2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform2dv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform2f ( GLint location_, GLfloat v0_, GLfloat v1_ ){
    static PFNGLUNIFORM2FPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2FPROC ) mygetprocaddr("glUniform2f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glUniform2f",params);
    }
    F(location_, v0_, v1_);
    return;
}
void glUniform2fv ( GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLUNIFORM2FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2FVPROC ) mygetprocaddr("glUniform2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform2fv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform2i ( GLint location_, GLint v0_, GLint v1_ ){
    static PFNGLUNIFORM2IPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2IPROC ) mygetprocaddr("glUniform2i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glUniform2i",params);
    }
    F(location_, v0_, v1_);
    return;
}
void glUniform2iv ( GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLUNIFORM2IVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2IVPROC ) mygetprocaddr("glUniform2iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform2iv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform2ui ( GLint location_, GLuint v0_, GLuint v1_ ){
    static PFNGLUNIFORM2UIPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2UIPROC ) mygetprocaddr("glUniform2ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        for(auto& it : wildcardHooks )
            it("glUniform2ui",params);
    }
    F(location_, v0_, v1_);
    return;
}
void glUniform2uiv ( GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLUNIFORM2UIVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM2UIVPROC ) mygetprocaddr("glUniform2uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform2uiv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform3d ( GLint location_, GLdouble x_, GLdouble y_, GLdouble z_ ){
    static PFNGLUNIFORM3DPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3DPROC ) mygetprocaddr("glUniform3d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glUniform3d",params);
    }
    F(location_, x_, y_, z_);
    return;
}
void glUniform3dv ( GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLUNIFORM3DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3DVPROC ) mygetprocaddr("glUniform3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform3dv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform3f ( GLint location_, GLfloat v0_, GLfloat v1_, GLfloat v2_ ){
    static PFNGLUNIFORM3FPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3FPROC ) mygetprocaddr("glUniform3f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glUniform3f",params);
    }
    F(location_, v0_, v1_, v2_);
    return;
}
void glUniform3fv ( GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLUNIFORM3FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3FVPROC ) mygetprocaddr("glUniform3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform3fv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform3i ( GLint location_, GLint v0_, GLint v1_, GLint v2_ ){
    static PFNGLUNIFORM3IPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3IPROC ) mygetprocaddr("glUniform3i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glUniform3i",params);
    }
    F(location_, v0_, v1_, v2_);
    return;
}
void glUniform3iv ( GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLUNIFORM3IVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3IVPROC ) mygetprocaddr("glUniform3iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform3iv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform3ui ( GLint location_, GLuint v0_, GLuint v1_, GLuint v2_ ){
    static PFNGLUNIFORM3UIPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3UIPROC ) mygetprocaddr("glUniform3ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        for(auto& it : wildcardHooks )
            it("glUniform3ui",params);
    }
    F(location_, v0_, v1_, v2_);
    return;
}
void glUniform3uiv ( GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLUNIFORM3UIVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM3UIVPROC ) mygetprocaddr("glUniform3uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform3uiv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform4d ( GLint location_, GLdouble x_, GLdouble y_, GLdouble z_, GLdouble w_ ){
    static PFNGLUNIFORM4DPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4DPROC ) mygetprocaddr("glUniform4d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glUniform4d",params);
    }
    F(location_, x_, y_, z_, w_);
    return;
}
void glUniform4dv ( GLint location_, GLsizei count_, const GLdouble * value_ ){
    static PFNGLUNIFORM4DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4DVPROC ) mygetprocaddr("glUniform4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform4dv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform4f ( GLint location_, GLfloat v0_, GLfloat v1_, GLfloat v2_, GLfloat v3_ ){
    static PFNGLUNIFORM4FPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4FPROC ) mygetprocaddr("glUniform4f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glUniform4f",params);
    }
    F(location_, v0_, v1_, v2_, v3_);
    return;
}
void glUniform4fv ( GLint location_, GLsizei count_, const GLfloat * value_ ){
    static PFNGLUNIFORM4FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4FVPROC ) mygetprocaddr("glUniform4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform4fv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform4i ( GLint location_, GLint v0_, GLint v1_, GLint v2_, GLint v3_ ){
    static PFNGLUNIFORM4IPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4IPROC ) mygetprocaddr("glUniform4i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glUniform4i",params);
    }
    F(location_, v0_, v1_, v2_, v3_);
    return;
}
void glUniform4iv ( GLint location_, GLsizei count_, const GLint * value_ ){
    static PFNGLUNIFORM4IVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4IVPROC ) mygetprocaddr("glUniform4iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform4iv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniform4ui ( GLint location_, GLuint v0_, GLuint v1_, GLuint v2_, GLuint v3_ ){
    static PFNGLUNIFORM4UIPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4UIPROC ) mygetprocaddr("glUniform4ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) v0_ );
        params.push_back( (uint64_t) v1_ );
        params.push_back( (uint64_t) v2_ );
        params.push_back( (uint64_t) v3_ );
        for(auto& it : wildcardHooks )
            it("glUniform4ui",params);
    }
    F(location_, v0_, v1_, v2_, v3_);
    return;
}
void glUniform4uiv ( GLint location_, GLsizei count_, const GLuint * value_ ){
    static PFNGLUNIFORM4UIVPROC F;
    if(!F)
        F = ( PFNGLUNIFORM4UIVPROC ) mygetprocaddr("glUniform4uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniform4uiv",params);
    }
    F(location_, count_, value_);
    return;
}
void glUniformBlockBinding ( GLuint program_, GLuint uniformBlockIndex_, GLuint uniformBlockBinding_ ){
    static PFNGLUNIFORMBLOCKBINDINGPROC F;
    if(!F)
        F = ( PFNGLUNIFORMBLOCKBINDINGPROC ) mygetprocaddr("glUniformBlockBinding");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        params.push_back( (uint64_t) uniformBlockIndex_ );
        params.push_back( (uint64_t) uniformBlockBinding_ );
        for(auto& it : wildcardHooks )
            it("glUniformBlockBinding",params);
    }
    F(program_, uniformBlockIndex_, uniformBlockBinding_);
    return;
}
void glUniformMatrix2dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX2DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX2DVPROC ) mygetprocaddr("glUniformMatrix2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix2dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix2fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX2FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX2FVPROC ) mygetprocaddr("glUniformMatrix2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix2fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix2x3dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX2X3DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX2X3DVPROC ) mygetprocaddr("glUniformMatrix2x3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix2x3dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix2x3fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX2X3FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX2X3FVPROC ) mygetprocaddr("glUniformMatrix2x3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix2x3fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix2x4dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX2X4DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX2X4DVPROC ) mygetprocaddr("glUniformMatrix2x4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix2x4dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix2x4fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX2X4FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX2X4FVPROC ) mygetprocaddr("glUniformMatrix2x4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix2x4fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix3dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX3DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX3DVPROC ) mygetprocaddr("glUniformMatrix3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix3dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix3fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX3FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX3FVPROC ) mygetprocaddr("glUniformMatrix3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix3fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix3x2dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX3X2DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX3X2DVPROC ) mygetprocaddr("glUniformMatrix3x2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix3x2dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix3x2fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX3X2FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX3X2FVPROC ) mygetprocaddr("glUniformMatrix3x2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix3x2fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix3x4dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX3X4DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX3X4DVPROC ) mygetprocaddr("glUniformMatrix3x4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix3x4dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix3x4fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX3X4FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX3X4FVPROC ) mygetprocaddr("glUniformMatrix3x4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix3x4fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix4dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX4DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX4DVPROC ) mygetprocaddr("glUniformMatrix4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix4dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix4fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX4FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX4FVPROC ) mygetprocaddr("glUniformMatrix4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix4fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix4x2dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX4X2DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX4X2DVPROC ) mygetprocaddr("glUniformMatrix4x2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix4x2dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix4x2fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX4X2FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX4X2FVPROC ) mygetprocaddr("glUniformMatrix4x2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix4x2fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix4x3dv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLdouble * value_ ){
    static PFNGLUNIFORMMATRIX4X3DVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX4X3DVPROC ) mygetprocaddr("glUniformMatrix4x3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix4x3dv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformMatrix4x3fv ( GLint location_, GLsizei count_, GLboolean transpose_, const GLfloat * value_ ){
    static PFNGLUNIFORMMATRIX4X3FVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMMATRIX4X3FVPROC ) mygetprocaddr("glUniformMatrix4x3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) location_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) transpose_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glUniformMatrix4x3fv",params);
    }
    F(location_, count_, transpose_, value_);
    return;
}
void glUniformSubroutinesuiv ( GLenum shadertype_, GLsizei count_, const GLuint * indices_ ){
    static PFNGLUNIFORMSUBROUTINESUIVPROC F;
    if(!F)
        F = ( PFNGLUNIFORMSUBROUTINESUIVPROC ) mygetprocaddr("glUniformSubroutinesuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) shadertype_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) indices_ );
        for(auto& it : wildcardHooks )
            it("glUniformSubroutinesuiv",params);
    }
    F(shadertype_, count_, indices_);
    return;
}
GLboolean glUnmapBuffer ( GLenum target_ ){
    static PFNGLUNMAPBUFFERPROC F;
    if(!F)
        F = ( PFNGLUNMAPBUFFERPROC ) mygetprocaddr("glUnmapBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) target_ );
        for(auto& it : wildcardHooks )
            it("glUnmapBuffer",params);
    }
    GLboolean retval = F(target_);
    return retval;
}
GLboolean glUnmapNamedBuffer ( GLuint buffer_ ){
    static PFNGLUNMAPNAMEDBUFFERPROC F;
    if(!F)
        F = ( PFNGLUNMAPNAMEDBUFFERPROC ) mygetprocaddr("glUnmapNamedBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glUnmapNamedBuffer",params);
    }
    GLboolean retval = F(buffer_);
    return retval;
}
void glUseProgram ( GLuint program_ ){
    static PFNGLUSEPROGRAMPROC F;
    if(!F)
        F = ( PFNGLUSEPROGRAMPROC ) mygetprocaddr("glUseProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glUseProgram",params);
    }
    F(program_);
    return;
}
void glUseProgramStages ( GLuint pipeline_, GLbitfield stages_, GLuint program_ ){
    static PFNGLUSEPROGRAMSTAGESPROC F;
    if(!F)
        F = ( PFNGLUSEPROGRAMSTAGESPROC ) mygetprocaddr("glUseProgramStages");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        params.push_back( (uint64_t) stages_ );
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glUseProgramStages",params);
    }
    F(pipeline_, stages_, program_);
    return;
}
void glValidateProgram ( GLuint program_ ){
    static PFNGLVALIDATEPROGRAMPROC F;
    if(!F)
        F = ( PFNGLVALIDATEPROGRAMPROC ) mygetprocaddr("glValidateProgram");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) program_ );
        for(auto& it : wildcardHooks )
            it("glValidateProgram",params);
    }
    F(program_);
    return;
}
void glValidateProgramPipeline ( GLuint pipeline_ ){
    static PFNGLVALIDATEPROGRAMPIPELINEPROC F;
    if(!F)
        F = ( PFNGLVALIDATEPROGRAMPIPELINEPROC ) mygetprocaddr("glValidateProgramPipeline");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) pipeline_ );
        for(auto& it : wildcardHooks )
            it("glValidateProgramPipeline",params);
    }
    F(pipeline_);
    return;
}
void glVertexArrayAttribBinding ( GLuint vaobj_, GLuint attribindex_, GLuint bindingindex_ ){
    static PFNGLVERTEXARRAYATTRIBBINDINGPROC F;
    if(!F)
        F = ( PFNGLVERTEXARRAYATTRIBBINDINGPROC ) mygetprocaddr("glVertexArrayAttribBinding");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) attribindex_ );
        params.push_back( (uint64_t) bindingindex_ );
        for(auto& it : wildcardHooks )
            it("glVertexArrayAttribBinding",params);
    }
    F(vaobj_, attribindex_, bindingindex_);
    return;
}
void glVertexArrayAttribFormat ( GLuint vaobj_, GLuint attribindex_, GLint size_, GLenum type_, GLboolean normalized_, GLuint relativeoffset_ ){
    static PFNGLVERTEXARRAYATTRIBFORMATPROC F;
    if(!F)
        F = ( PFNGLVERTEXARRAYATTRIBFORMATPROC ) mygetprocaddr("glVertexArrayAttribFormat");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) attribindex_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) relativeoffset_ );
        for(auto& it : wildcardHooks )
            it("glVertexArrayAttribFormat",params);
    }
    F(vaobj_, attribindex_, size_, type_, normalized_, relativeoffset_);
    return;
}
void glVertexArrayBindingDivisor ( GLuint vaobj_, GLuint bindingindex_, GLuint divisor_ ){
    static PFNGLVERTEXARRAYBINDINGDIVISORPROC F;
    if(!F)
        F = ( PFNGLVERTEXARRAYBINDINGDIVISORPROC ) mygetprocaddr("glVertexArrayBindingDivisor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) bindingindex_ );
        params.push_back( (uint64_t) divisor_ );
        for(auto& it : wildcardHooks )
            it("glVertexArrayBindingDivisor",params);
    }
    F(vaobj_, bindingindex_, divisor_);
    return;
}
void glVertexArrayElementBuffer ( GLuint vaobj_, GLuint buffer_ ){
    static PFNGLVERTEXARRAYELEMENTBUFFERPROC F;
    if(!F)
        F = ( PFNGLVERTEXARRAYELEMENTBUFFERPROC ) mygetprocaddr("glVertexArrayElementBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) buffer_ );
        for(auto& it : wildcardHooks )
            it("glVertexArrayElementBuffer",params);
    }
    F(vaobj_, buffer_);
    return;
}
void glVertexArrayVertexBuffer ( GLuint vaobj_, GLuint bindingindex_, GLuint buffer_, GLintptr offset_, GLsizei stride_ ){
    static PFNGLVERTEXARRAYVERTEXBUFFERPROC F;
    if(!F)
        F = ( PFNGLVERTEXARRAYVERTEXBUFFERPROC ) mygetprocaddr("glVertexArrayVertexBuffer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) bindingindex_ );
        params.push_back( (uint64_t) buffer_ );
        params.push_back( (uint64_t) offset_ );
        params.push_back( (uint64_t) stride_ );
        for(auto& it : wildcardHooks )
            it("glVertexArrayVertexBuffer",params);
    }
    F(vaobj_, bindingindex_, buffer_, offset_, stride_);
    return;
}
void glVertexArrayVertexBuffers ( GLuint vaobj_, GLuint first_, GLsizei count_, const GLuint * buffers_, const GLintptr * offsets_, const GLsizei * strides_ ){
    static PFNGLVERTEXARRAYVERTEXBUFFERSPROC F;
    if(!F)
        F = ( PFNGLVERTEXARRAYVERTEXBUFFERSPROC ) mygetprocaddr("glVertexArrayVertexBuffers");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) vaobj_ );
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) buffers_ );
        params.push_back( (uint64_t) offsets_ );
        params.push_back( (uint64_t) strides_ );
        for(auto& it : wildcardHooks )
            it("glVertexArrayVertexBuffers",params);
    }
    F(vaobj_, first_, count_, buffers_, offsets_, strides_);
    return;
}
void glVertexAttrib1d ( GLuint index_, GLdouble x_ ){
    static PFNGLVERTEXATTRIB1DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB1DPROC ) mygetprocaddr("glVertexAttrib1d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib1d",params);
    }
    F(index_, x_);
    return;
}
void glVertexAttrib1dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIB1DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB1DVPROC ) mygetprocaddr("glVertexAttrib1dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib1dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib1f ( GLuint index_, GLfloat x_ ){
    static PFNGLVERTEXATTRIB1FPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB1FPROC ) mygetprocaddr("glVertexAttrib1f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib1f",params);
    }
    F(index_, x_);
    return;
}
void glVertexAttrib1fv ( GLuint index_, const GLfloat * v_ ){
    static PFNGLVERTEXATTRIB1FVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB1FVPROC ) mygetprocaddr("glVertexAttrib1fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib1fv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib1s ( GLuint index_, GLshort x_ ){
    static PFNGLVERTEXATTRIB1SPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB1SPROC ) mygetprocaddr("glVertexAttrib1s");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib1s",params);
    }
    F(index_, x_);
    return;
}
void glVertexAttrib1sv ( GLuint index_, const GLshort * v_ ){
    static PFNGLVERTEXATTRIB1SVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB1SVPROC ) mygetprocaddr("glVertexAttrib1sv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib1sv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib2d ( GLuint index_, GLdouble x_, GLdouble y_ ){
    static PFNGLVERTEXATTRIB2DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB2DPROC ) mygetprocaddr("glVertexAttrib2d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib2d",params);
    }
    F(index_, x_, y_);
    return;
}
void glVertexAttrib2dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIB2DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB2DVPROC ) mygetprocaddr("glVertexAttrib2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib2dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib2f ( GLuint index_, GLfloat x_, GLfloat y_ ){
    static PFNGLVERTEXATTRIB2FPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB2FPROC ) mygetprocaddr("glVertexAttrib2f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib2f",params);
    }
    F(index_, x_, y_);
    return;
}
void glVertexAttrib2fv ( GLuint index_, const GLfloat * v_ ){
    static PFNGLVERTEXATTRIB2FVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB2FVPROC ) mygetprocaddr("glVertexAttrib2fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib2fv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib2s ( GLuint index_, GLshort x_, GLshort y_ ){
    static PFNGLVERTEXATTRIB2SPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB2SPROC ) mygetprocaddr("glVertexAttrib2s");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib2s",params);
    }
    F(index_, x_, y_);
    return;
}
void glVertexAttrib2sv ( GLuint index_, const GLshort * v_ ){
    static PFNGLVERTEXATTRIB2SVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB2SVPROC ) mygetprocaddr("glVertexAttrib2sv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib2sv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib3d ( GLuint index_, GLdouble x_, GLdouble y_, GLdouble z_ ){
    static PFNGLVERTEXATTRIB3DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB3DPROC ) mygetprocaddr("glVertexAttrib3d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib3d",params);
    }
    F(index_, x_, y_, z_);
    return;
}
void glVertexAttrib3dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIB3DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB3DVPROC ) mygetprocaddr("glVertexAttrib3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib3dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib3f ( GLuint index_, GLfloat x_, GLfloat y_, GLfloat z_ ){
    static PFNGLVERTEXATTRIB3FPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB3FPROC ) mygetprocaddr("glVertexAttrib3f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib3f",params);
    }
    F(index_, x_, y_, z_);
    return;
}
void glVertexAttrib3fv ( GLuint index_, const GLfloat * v_ ){
    static PFNGLVERTEXATTRIB3FVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB3FVPROC ) mygetprocaddr("glVertexAttrib3fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib3fv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib3s ( GLuint index_, GLshort x_, GLshort y_, GLshort z_ ){
    static PFNGLVERTEXATTRIB3SPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB3SPROC ) mygetprocaddr("glVertexAttrib3s");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib3s",params);
    }
    F(index_, x_, y_, z_);
    return;
}
void glVertexAttrib3sv ( GLuint index_, const GLshort * v_ ){
    static PFNGLVERTEXATTRIB3SVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB3SVPROC ) mygetprocaddr("glVertexAttrib3sv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib3sv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4Nbv ( GLuint index_, const GLbyte * v_ ){
    static PFNGLVERTEXATTRIB4NBVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NBVPROC ) mygetprocaddr("glVertexAttrib4Nbv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Nbv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4Niv ( GLuint index_, const GLint * v_ ){
    static PFNGLVERTEXATTRIB4NIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NIVPROC ) mygetprocaddr("glVertexAttrib4Niv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Niv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4Nsv ( GLuint index_, const GLshort * v_ ){
    static PFNGLVERTEXATTRIB4NSVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NSVPROC ) mygetprocaddr("glVertexAttrib4Nsv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Nsv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4Nub ( GLuint index_, GLubyte x_, GLubyte y_, GLubyte z_, GLubyte w_ ){
    static PFNGLVERTEXATTRIB4NUBPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NUBPROC ) mygetprocaddr("glVertexAttrib4Nub");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Nub",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttrib4Nubv ( GLuint index_, const GLubyte * v_ ){
    static PFNGLVERTEXATTRIB4NUBVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NUBVPROC ) mygetprocaddr("glVertexAttrib4Nubv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Nubv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4Nuiv ( GLuint index_, const GLuint * v_ ){
    static PFNGLVERTEXATTRIB4NUIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NUIVPROC ) mygetprocaddr("glVertexAttrib4Nuiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Nuiv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4Nusv ( GLuint index_, const GLushort * v_ ){
    static PFNGLVERTEXATTRIB4NUSVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4NUSVPROC ) mygetprocaddr("glVertexAttrib4Nusv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4Nusv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4bv ( GLuint index_, const GLbyte * v_ ){
    static PFNGLVERTEXATTRIB4BVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4BVPROC ) mygetprocaddr("glVertexAttrib4bv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4bv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4d ( GLuint index_, GLdouble x_, GLdouble y_, GLdouble z_, GLdouble w_ ){
    static PFNGLVERTEXATTRIB4DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4DPROC ) mygetprocaddr("glVertexAttrib4d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4d",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttrib4dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIB4DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4DVPROC ) mygetprocaddr("glVertexAttrib4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4f ( GLuint index_, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_ ){
    static PFNGLVERTEXATTRIB4FPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4FPROC ) mygetprocaddr("glVertexAttrib4f");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4f",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttrib4fv ( GLuint index_, const GLfloat * v_ ){
    static PFNGLVERTEXATTRIB4FVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4FVPROC ) mygetprocaddr("glVertexAttrib4fv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4fv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4iv ( GLuint index_, const GLint * v_ ){
    static PFNGLVERTEXATTRIB4IVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4IVPROC ) mygetprocaddr("glVertexAttrib4iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4iv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4s ( GLuint index_, GLshort x_, GLshort y_, GLshort z_, GLshort w_ ){
    static PFNGLVERTEXATTRIB4SPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4SPROC ) mygetprocaddr("glVertexAttrib4s");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4s",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttrib4sv ( GLuint index_, const GLshort * v_ ){
    static PFNGLVERTEXATTRIB4SVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4SVPROC ) mygetprocaddr("glVertexAttrib4sv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4sv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4ubv ( GLuint index_, const GLubyte * v_ ){
    static PFNGLVERTEXATTRIB4UBVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4UBVPROC ) mygetprocaddr("glVertexAttrib4ubv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4ubv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4uiv ( GLuint index_, const GLuint * v_ ){
    static PFNGLVERTEXATTRIB4UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4UIVPROC ) mygetprocaddr("glVertexAttrib4uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4uiv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttrib4usv ( GLuint index_, const GLushort * v_ ){
    static PFNGLVERTEXATTRIB4USVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIB4USVPROC ) mygetprocaddr("glVertexAttrib4usv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttrib4usv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribBinding ( GLuint attribindex_, GLuint bindingindex_ ){
    static PFNGLVERTEXATTRIBBINDINGPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBBINDINGPROC ) mygetprocaddr("glVertexAttribBinding");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) attribindex_ );
        params.push_back( (uint64_t) bindingindex_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribBinding",params);
    }
    F(attribindex_, bindingindex_);
    return;
}
void glVertexAttribDivisor ( GLuint index_, GLuint divisor_ ){
    static PFNGLVERTEXATTRIBDIVISORPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBDIVISORPROC ) mygetprocaddr("glVertexAttribDivisor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) divisor_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribDivisor",params);
    }
    F(index_, divisor_);
    return;
}
void glVertexAttribFormat ( GLuint attribindex_, GLint size_, GLenum type_, GLboolean normalized_, GLuint relativeoffset_ ){
    static PFNGLVERTEXATTRIBFORMATPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBFORMATPROC ) mygetprocaddr("glVertexAttribFormat");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) attribindex_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) relativeoffset_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribFormat",params);
    }
    F(attribindex_, size_, type_, normalized_, relativeoffset_);
    return;
}
void glVertexAttribI1i ( GLuint index_, GLint x_ ){
    static PFNGLVERTEXATTRIBI1IPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI1IPROC ) mygetprocaddr("glVertexAttribI1i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI1i",params);
    }
    F(index_, x_);
    return;
}
void glVertexAttribI1iv ( GLuint index_, const GLint * v_ ){
    static PFNGLVERTEXATTRIBI1IVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI1IVPROC ) mygetprocaddr("glVertexAttribI1iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI1iv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI1ui ( GLuint index_, GLuint x_ ){
    static PFNGLVERTEXATTRIBI1UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI1UIPROC ) mygetprocaddr("glVertexAttribI1ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI1ui",params);
    }
    F(index_, x_);
    return;
}
void glVertexAttribI1uiv ( GLuint index_, const GLuint * v_ ){
    static PFNGLVERTEXATTRIBI1UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI1UIVPROC ) mygetprocaddr("glVertexAttribI1uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI1uiv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI2i ( GLuint index_, GLint x_, GLint y_ ){
    static PFNGLVERTEXATTRIBI2IPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI2IPROC ) mygetprocaddr("glVertexAttribI2i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI2i",params);
    }
    F(index_, x_, y_);
    return;
}
void glVertexAttribI2iv ( GLuint index_, const GLint * v_ ){
    static PFNGLVERTEXATTRIBI2IVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI2IVPROC ) mygetprocaddr("glVertexAttribI2iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI2iv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI2ui ( GLuint index_, GLuint x_, GLuint y_ ){
    static PFNGLVERTEXATTRIBI2UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI2UIPROC ) mygetprocaddr("glVertexAttribI2ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI2ui",params);
    }
    F(index_, x_, y_);
    return;
}
void glVertexAttribI2uiv ( GLuint index_, const GLuint * v_ ){
    static PFNGLVERTEXATTRIBI2UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI2UIVPROC ) mygetprocaddr("glVertexAttribI2uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI2uiv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI3i ( GLuint index_, GLint x_, GLint y_, GLint z_ ){
    static PFNGLVERTEXATTRIBI3IPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI3IPROC ) mygetprocaddr("glVertexAttribI3i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI3i",params);
    }
    F(index_, x_, y_, z_);
    return;
}
void glVertexAttribI3iv ( GLuint index_, const GLint * v_ ){
    static PFNGLVERTEXATTRIBI3IVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI3IVPROC ) mygetprocaddr("glVertexAttribI3iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI3iv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI3ui ( GLuint index_, GLuint x_, GLuint y_, GLuint z_ ){
    static PFNGLVERTEXATTRIBI3UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI3UIPROC ) mygetprocaddr("glVertexAttribI3ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI3ui",params);
    }
    F(index_, x_, y_, z_);
    return;
}
void glVertexAttribI3uiv ( GLuint index_, const GLuint * v_ ){
    static PFNGLVERTEXATTRIBI3UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI3UIVPROC ) mygetprocaddr("glVertexAttribI3uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI3uiv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI4bv ( GLuint index_, const GLbyte * v_ ){
    static PFNGLVERTEXATTRIBI4BVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4BVPROC ) mygetprocaddr("glVertexAttribI4bv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4bv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI4i ( GLuint index_, GLint x_, GLint y_, GLint z_, GLint w_ ){
    static PFNGLVERTEXATTRIBI4IPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4IPROC ) mygetprocaddr("glVertexAttribI4i");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4i",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttribI4iv ( GLuint index_, const GLint * v_ ){
    static PFNGLVERTEXATTRIBI4IVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4IVPROC ) mygetprocaddr("glVertexAttribI4iv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4iv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI4sv ( GLuint index_, const GLshort * v_ ){
    static PFNGLVERTEXATTRIBI4SVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4SVPROC ) mygetprocaddr("glVertexAttribI4sv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4sv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI4ubv ( GLuint index_, const GLubyte * v_ ){
    static PFNGLVERTEXATTRIBI4UBVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4UBVPROC ) mygetprocaddr("glVertexAttribI4ubv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4ubv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI4ui ( GLuint index_, GLuint x_, GLuint y_, GLuint z_, GLuint w_ ){
    static PFNGLVERTEXATTRIBI4UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4UIPROC ) mygetprocaddr("glVertexAttribI4ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4ui",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttribI4uiv ( GLuint index_, const GLuint * v_ ){
    static PFNGLVERTEXATTRIBI4UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4UIVPROC ) mygetprocaddr("glVertexAttribI4uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4uiv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribI4usv ( GLuint index_, const GLushort * v_ ){
    static PFNGLVERTEXATTRIBI4USVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBI4USVPROC ) mygetprocaddr("glVertexAttribI4usv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribI4usv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribL1d ( GLuint index_, GLdouble x_ ){
    static PFNGLVERTEXATTRIBL1DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL1DPROC ) mygetprocaddr("glVertexAttribL1d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL1d",params);
    }
    F(index_, x_);
    return;
}
void glVertexAttribL1dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIBL1DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL1DVPROC ) mygetprocaddr("glVertexAttribL1dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL1dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribL2d ( GLuint index_, GLdouble x_, GLdouble y_ ){
    static PFNGLVERTEXATTRIBL2DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL2DPROC ) mygetprocaddr("glVertexAttribL2d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL2d",params);
    }
    F(index_, x_, y_);
    return;
}
void glVertexAttribL2dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIBL2DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL2DVPROC ) mygetprocaddr("glVertexAttribL2dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL2dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribL3d ( GLuint index_, GLdouble x_, GLdouble y_, GLdouble z_ ){
    static PFNGLVERTEXATTRIBL3DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL3DPROC ) mygetprocaddr("glVertexAttribL3d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL3d",params);
    }
    F(index_, x_, y_, z_);
    return;
}
void glVertexAttribL3dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIBL3DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL3DVPROC ) mygetprocaddr("glVertexAttribL3dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL3dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribL4d ( GLuint index_, GLdouble x_, GLdouble y_, GLdouble z_, GLdouble w_ ){
    static PFNGLVERTEXATTRIBL4DPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL4DPROC ) mygetprocaddr("glVertexAttribL4d");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) z_ );
        params.push_back( (uint64_t) w_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL4d",params);
    }
    F(index_, x_, y_, z_, w_);
    return;
}
void glVertexAttribL4dv ( GLuint index_, const GLdouble * v_ ){
    static PFNGLVERTEXATTRIBL4DVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBL4DVPROC ) mygetprocaddr("glVertexAttribL4dv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribL4dv",params);
    }
    F(index_, v_);
    return;
}
void glVertexAttribP1ui ( GLuint index_, GLenum type_, GLboolean normalized_, GLuint value_ ){
    static PFNGLVERTEXATTRIBP1UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP1UIPROC ) mygetprocaddr("glVertexAttribP1ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP1ui",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP1uiv ( GLuint index_, GLenum type_, GLboolean normalized_, const GLuint * value_ ){
    static PFNGLVERTEXATTRIBP1UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP1UIVPROC ) mygetprocaddr("glVertexAttribP1uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP1uiv",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP2ui ( GLuint index_, GLenum type_, GLboolean normalized_, GLuint value_ ){
    static PFNGLVERTEXATTRIBP2UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP2UIPROC ) mygetprocaddr("glVertexAttribP2ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP2ui",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP2uiv ( GLuint index_, GLenum type_, GLboolean normalized_, const GLuint * value_ ){
    static PFNGLVERTEXATTRIBP2UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP2UIVPROC ) mygetprocaddr("glVertexAttribP2uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP2uiv",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP3ui ( GLuint index_, GLenum type_, GLboolean normalized_, GLuint value_ ){
    static PFNGLVERTEXATTRIBP3UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP3UIPROC ) mygetprocaddr("glVertexAttribP3ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP3ui",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP3uiv ( GLuint index_, GLenum type_, GLboolean normalized_, const GLuint * value_ ){
    static PFNGLVERTEXATTRIBP3UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP3UIVPROC ) mygetprocaddr("glVertexAttribP3uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP3uiv",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP4ui ( GLuint index_, GLenum type_, GLboolean normalized_, GLuint value_ ){
    static PFNGLVERTEXATTRIBP4UIPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP4UIPROC ) mygetprocaddr("glVertexAttribP4ui");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP4ui",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribP4uiv ( GLuint index_, GLenum type_, GLboolean normalized_, const GLuint * value_ ){
    static PFNGLVERTEXATTRIBP4UIVPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBP4UIVPROC ) mygetprocaddr("glVertexAttribP4uiv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) value_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribP4uiv",params);
    }
    F(index_, type_, normalized_, value_);
    return;
}
void glVertexAttribPointer ( GLuint index_, GLint size_, GLenum type_, GLboolean normalized_, GLsizei stride_, const void * pointer_ ){
    static PFNGLVERTEXATTRIBPOINTERPROC F;
    if(!F)
        F = ( PFNGLVERTEXATTRIBPOINTERPROC ) mygetprocaddr("glVertexAttribPointer");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) size_ );
        params.push_back( (uint64_t) type_ );
        params.push_back( (uint64_t) normalized_ );
        params.push_back( (uint64_t) stride_ );
        params.push_back( (uint64_t) pointer_ );
        for(auto& it : wildcardHooks )
            it("glVertexAttribPointer",params);
    }
    F(index_, size_, type_, normalized_, stride_, pointer_);
    return;
}
void glVertexBindingDivisor ( GLuint bindingindex_, GLuint divisor_ ){
    static PFNGLVERTEXBINDINGDIVISORPROC F;
    if(!F)
        F = ( PFNGLVERTEXBINDINGDIVISORPROC ) mygetprocaddr("glVertexBindingDivisor");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) bindingindex_ );
        params.push_back( (uint64_t) divisor_ );
        for(auto& it : wildcardHooks )
            it("glVertexBindingDivisor",params);
    }
    F(bindingindex_, divisor_);
    return;
}
void glViewport ( GLint x_, GLint y_, GLsizei width_, GLsizei height_ ){
    static PFNGLVIEWPORTPROC F;
    if(!F)
        F = ( PFNGLVIEWPORTPROC ) mygetprocaddr("glViewport");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) width_ );
        params.push_back( (uint64_t) height_ );
        for(auto& it : wildcardHooks )
            it("glViewport",params);
    }
    F(x_, y_, width_, height_);
    return;
}
void glViewportArrayv ( GLuint first_, GLsizei count_, const GLfloat * v_ ){
    static PFNGLVIEWPORTARRAYVPROC F;
    if(!F)
        F = ( PFNGLVIEWPORTARRAYVPROC ) mygetprocaddr("glViewportArrayv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) first_ );
        params.push_back( (uint64_t) count_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glViewportArrayv",params);
    }
    F(first_, count_, v_);
    return;
}
void glViewportIndexedf ( GLuint index_, GLfloat x_, GLfloat y_, GLfloat w_, GLfloat h_ ){
    static PFNGLVIEWPORTINDEXEDFPROC F;
    if(!F)
        F = ( PFNGLVIEWPORTINDEXEDFPROC ) mygetprocaddr("glViewportIndexedf");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) x_ );
        params.push_back( (uint64_t) y_ );
        params.push_back( (uint64_t) w_ );
        params.push_back( (uint64_t) h_ );
        for(auto& it : wildcardHooks )
            it("glViewportIndexedf",params);
    }
    F(index_, x_, y_, w_, h_);
    return;
}
void glViewportIndexedfv ( GLuint index_, const GLfloat * v_ ){
    static PFNGLVIEWPORTINDEXEDFVPROC F;
    if(!F)
        F = ( PFNGLVIEWPORTINDEXEDFVPROC ) mygetprocaddr("glViewportIndexedfv");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) index_ );
        params.push_back( (uint64_t) v_ );
        for(auto& it : wildcardHooks )
            it("glViewportIndexedfv",params);
    }
    F(index_, v_);
    return;
}
void glWaitSync ( GLsync sync_, GLbitfield flags_, GLuint64 timeout_ ){
    static PFNGLWAITSYNCPROC F;
    if(!F)
        F = ( PFNGLWAITSYNCPROC ) mygetprocaddr("glWaitSync");
    if( !wildcardHooks.empty() ){
        std::vector<uint64_t> params;
        params.push_back( (uint64_t) sync_ );
        params.push_back( (uint64_t) flags_ );
        params.push_back( (uint64_t) timeout_ );
        for(auto& it : wildcardHooks )
            it("glWaitSync",params);
    }
    F(sync_, flags_, timeout_);
    return;
}
