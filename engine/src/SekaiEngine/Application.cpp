#include "SekaiEngine/Application.h"

namespace SekaiEngine
{
    Application::Application()
        :window(IWindow::Create())
    {
        
    }
    Application::Application(const Application& app)
        :window(app.window)
    {

    }

    Application::~Application()
    {
        delete window;
    }

    void Application::Run()
    {
        while(true)
        {
            window->OnUpdate();
        }
    }
} // namespace SekaiEngine
