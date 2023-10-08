#include <utility>
#include "BeginnerWindow.h"

Beginner::BeginnerWindow::BeginnerWindow()
{
    this->_width = DEFAULT_WINDOW_WIDTH;
    this->_height = DEFAULT_WINDOW_HEIGHT;
    this->_window_name = "Default Beginner Window";
    cout << "[Beginner][Window] iii" << endl;
}

Beginner::BeginnerWindow::BeginnerWindow(int width, int height, string name)
{
    this->_width = width;
    this->_height = height;
    this->_window_name = std::move(name);
}

bool Beginner::BeginnerWindow::IsWindowActive() const
{
    return !glfwWindowShouldClose(_window);
}

void Beginner::BeginnerWindow::Tick() const
{
    TickRender();
    TickWindowFlash();
}

void Beginner::BeginnerWindow::ChangeDisplayMode(DisplayMode displayMode)
{
    switch (displayMode)
    {
    case Shaded: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    case WireFrame: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

bool Beginner::BeginnerWindow::InitializeWindow()
{
    cout << "[Beginner][Window] start initialize" << endl;

    glfwSetErrorCallback(WindowInitErrorCallback);

    if (!glfwInit())
    {
        return false;
    }

    // ���ð汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const char* windowNameC = _window_name.c_str();
    cout << "[Beginner][Window] create window " << windowNameC << " width X height " << _width << " X " << _height <<
        endl;
    _window = glfwCreateWindow(_width, _height, windowNameC, nullptr, nullptr);

    if (!_window)
    {
        cout << "[Beginner][Window] window creat failed" << endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(_window); // TODO: �Ƿ�����Ҫ�ര�ڵ����

    // ע������¼�
    // glfwSetFramebufferSizeCallback(window, on_framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, on_mouse_move_callback);
    // glfwSetScrollCallback(window, on_mouse_scroll_callback);

    // ��������״̬���Ƿ��������ģ�
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // һ��Ҫ��ʼ�� GLAD ����������������������������������
    if (!gladLoadGL())
    {
        cout << "GLAD���غ���ʧ��" << endl;
        return false;
    }
    
    _base_fbo = new FrameBuffer(_width, _height);

    cout << "[Beginner] [Window] initialize sequence end" << endl;
    return true;
}

void Beginner::BeginnerWindow::WindowInitErrorCallback(const int error, const char* msg)
{
    const std::string s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}

void Beginner::BeginnerWindow::Terminate() const
{
    TickWindowFlash();
}

#pragma region Private

void Beginner::BeginnerWindow::TickWindowFlash() const
{
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Beginner::BeginnerWindow::TickRender() const
{
    _base_fbo->Enable();
    
    // �����Ļ
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    _base_fbo->Disable();
}

#pragma endregion

Beginner::BeginnerWindow::~BeginnerWindow()
{
    cout << "[Beginner] [Window] window destroy" << endl;
    glfwDestroyWindow(_window);
    glfwTerminate();

    delete _base_fbo;
}