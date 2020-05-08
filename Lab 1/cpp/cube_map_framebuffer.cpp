#include <stdafx.h>
#include <cube_map_framebuffer.h>

CubeMapFramebuffer::CubeMapFramebuffer(unsigned size, unsigned channels, GLenum format)
{
	this->width = size;
	this->height = size;
	this->channels = channels;
	this->format = format;

	glGenFramebuffers(1, &FBO);	//Create framebuffer
	glGenTextures(1, &textureID);	//create depth cubemap texture

	//Bind cubemap texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (int i = 0; i < 6; i++)

		//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
		//	this->width, this->height, 0, GL_DEPTH_COMPONENT, this->format, NULL);

	//Set Parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CubeMapFramebuffer::~CubeMapFramebuffer()
{
	glDeleteFramebuffers(1, &FBO);
}

void CubeMapFramebuffer::setAsRenderTarget(bool clearIt)
{
	glViewport(0, 0, this->width, this->height);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	if(clearIt)
		glClear(GL_DEPTH_BUFFER_BIT);
}

void CubeMapFramebuffer::unsetAsRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeMapFramebuffer::dump(std::string filename)
{
	unsigned chunkSize = this->width * this->height * 4;

	std::vector<char> B(chunkSize);
	this->texture->bind(0);
	glGetTexImage(GL_TEXTURE_CUBE_MAP, 0, GL_RGBA, GL_UNSIGNED_BYTE, B.data());
	this->texture->unbind(0);
	char* p = B.data();
	for (int i = 0; i < 1; i++)
	{
		Image img(this->width, this->height, "RGBA8");
		std::memcpy(img.pixels(), p, chunkSize);
		p += chunkSize;
		std::string fn = filename + "-texture-1-slice-1.png";
		img.writePng(fn);
		std::cout << "Wrote " << fn << "\n";
	}
}

CubeMapFramebuffer::AttachmentInfo::AttachmentInfo(unsigned count, GLenum fmt)
{
	this->format = fmt;
	this->slices = count;
	if( count > 16 )
		throw std::runtime_error("Bad count for attachments");
}