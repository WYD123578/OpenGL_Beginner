#include "Beginner/Core/Render/Window/BeginnerWindow.h"
#include "Beginner/Tool/CameraControl.h"
#include "Beginner/Tool/ImGuiWindow.h"

#define AssertIf(x) do { sizeof(x); } while (0);

int main()
{
    const auto window = new Beginner::BeginnerWindow();
    const auto camera = new CameraControl(glm::vec3(-2.5f, 2.8f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -25.0f, 35.0f);
    
    // Window module
    if (!window->InitializeWindow())
    {
        cout << "[Beginner][Core] window initialize failed, exit" << endl;
        delete window;
        return 1;
    }
    
    // ����imGui����
    // const auto im_gui_win = new ImGuiWindow(Beginner::BeginnerWindow::window, *camera);
    
    // ��ѭ������Ϸ���嶼�������ѭ��������֡����
    while (window->IsWindowActive())
    {
        window->Tick();
        // im_gui_win->render();
    }

    window->Terminate();
    
    delete camera;
    delete window;

    std::cin.get();

    return 0;
}
