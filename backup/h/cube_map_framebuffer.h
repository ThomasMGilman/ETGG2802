#pragma once
#include <stdafx.h>

class CubeMapFramebuffer
{
private:
	unsigned width, height, channels, format;
	GLuint FBO, textureID;
protected:
public:
	class AttachmentInfo {
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

	CubeMapFramebuffer(unsigned size, unsigned channels, GLenum format);

	~CubeMapFramebuffer();

	void setAsRenderTarget(bool clearIt);

	void unsetAsRenderTarget();

	void dump(std::string filename);
};