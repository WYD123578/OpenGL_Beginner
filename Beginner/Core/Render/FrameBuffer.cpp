#include "FrameBuffer.h"

void FrameBuffer::SetTextureColorBuffer(const int width, const int height) const
{
    glBindTexture(GL_TEXTURE_2D, _textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::SetRenderBuffer(const int width, const int height)
{
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::FrameBuffer(const int width, const int height) : _fbo(0), _textureColorBuffer(0)
{
    glGenFramebuffers(1, &_fbo);
    glGenTextures(1, &_textureColorBuffer);
    
    // 绑定渲染帧缓冲，并检查其完整性
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    SetTextureColorBuffer(width, height);       // 必须绑定一个颜色缓冲
    SetRenderBuffer(width,height);              // 必须绑定一个渲染缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, DISABLE_FBO_ID);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[Error Frame Buffer] Frame Buffer is not complete\n";
        return;
    }
}

void FrameBuffer::Enable() const
{
    // 应该0表示什么都没有绑定
    if (_fbo == DISABLE_FBO_ID)
    {
        std::cout << "[Beginner][FrameBuffer] try to enable null fbo" << std::endl;
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void FrameBuffer::Disable() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, DISABLE_FBO_ID);
}


FrameBuffer::~FrameBuffer()
{
    if (_fbo != DISABLE_FBO_ID) {
        glDeleteFramebuffers(1, &_fbo);
    }
}
