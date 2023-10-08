#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define DISABLE_FBO_ID 0

class FrameBuffer
{
private:
    GLuint _fbo;
    GLuint _textureColorBuffer;

    void SetTextureColorBuffer(const int width, const int height) const;
    static void SetRenderBuffer(const int width, const int height);

public:
    FrameBuffer(int width, int height);

    void Enable() const;
    void Disable() const;

    ~FrameBuffer();
};
