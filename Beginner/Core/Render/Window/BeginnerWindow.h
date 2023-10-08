#pragma once

#include <iostream>
#include <string>
#include "Beginner/Core/Render/FrameBuffer.h"


using namespace std;

namespace Beginner
{
    enum DisplayMode
    {
        Shaded = 0,
        WireFrame = 1,
    };

    class BeginnerWindow
    {
    public:
        const int DEFAULT_WINDOW_WIDTH = 800;
        const int DEFAULT_WINDOW_HEIGHT = 480;

        BeginnerWindow();
        BeginnerWindow(int with, int height, string name = "Beginner");

        void SetWidth();
        void SetHeight();

        int GetWidth();
        int GetHeight();

        void Tick() const;

        bool IsWindowActive() const;
        static void ChangeDisplayMode(DisplayMode displayMode);

        bool InitializeWindow();


        void Terminate() const;

        ~BeginnerWindow();

    private:
        GLFWwindow* _window;

        int _width;
        int _height;
        string _window_name;

        FrameBuffer* _base_fbo;

        /// <summary>
        /// 初始化window可能会失败，这个时候做些处理
        /// </summary>
        /// <param name="error">error code</param>
        /// <param name="msg">error message</param>
        static void WindowInitErrorCallback(int error, const char* msg);

#pragma region Tick

        void TickWindowFlash() const;
        void TickWindowInput(float deltaTime);
        void TickKeyEvent();
        void TickRender() const;

#pragma endregion
    };
}
