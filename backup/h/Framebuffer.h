#pragma once

#include "Texture2DArray.h"
#include <string>
#include <vector>
#include <memory>
#include "glfuncs.h"

class Globals;

class Framebuffer{
    friend Globals;
public:
    int width, height;
    GLuint fbo;
    
    class AttachmentInfo{
      public:
        unsigned slices;
        GLenum format;
        AttachmentInfo(unsigned count, GLenum fmt);
    };
        
        
    /** All the color textures of this FBO*/
    std::vector<std::shared_ptr<Texture2DArray>> textures; 
    
    /** An alias for textures[0] */
    std::shared_ptr<Texture2DArray> texture;

    /** This FBO's depth buffer */
    std::shared_ptr<Texture2DArray> depthtexture;
        
    /** Creates a separate texture for each format*/
    Framebuffer(int width, int height, const std::initializer_list<GLenum>& formats);
    
    /** Creates one texture with the given number of slices */
    Framebuffer(int width, int height, unsigned slices, GLenum format );
    
    /** Allows specification of the number of slices per texture and the format of each texture. */
    Framebuffer(int width, int height, const std::vector<AttachmentInfo>& formats );
    
    ~Framebuffer();
    
    /** All future rendering will go to this framebuffer.
        @param clearIt  Perform a glClear() on this fbo.
    */
    void setAsRenderTarget(bool clearIt);
    
    /** All future rendering will go to the screen. */
    void unsetAsRenderTarget();
    
    /** Debugging: Output the contents of this FBO to one or more files.*/
    void dump(std::string filename);
    
    /** Blur this fbo. Note that this *does* respect the current stencil test mode.
        @param attachmentIndex Which layer to blur
        @param radius The blur radius
        @param multiplier This value is multiplied with each pixel value to brighten or dim it.
    */
    void blur(unsigned textureNumber, unsigned sliceNumber, int radius, float multiplier, int isHDR);

    void copyTo(Framebuffer& otherFBO);
  
  private:
    std::vector<GLenum> drawbuffers;
    std::vector<GLenum> formats;
    
    void init(int width, int height, const std::vector<AttachmentInfo>& formats );
    
    static Framebuffer* current;
    
    Framebuffer(const Framebuffer&) = delete;
    void operator=(const Framebuffer&) = delete;
};

