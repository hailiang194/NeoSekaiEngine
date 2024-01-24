#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Event/WindowEvent.h"
#include "SekaiEngine/Application.h"

namespace SekaiEngine
{
    Application::Application()
        :window(IWindow::Create()), m_running(true)
    {
        window->setEventCallbackFn(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    }
    Application::Application(const Application& app)
        :window(app.window), m_running(app.m_running)
    {

    }

    Application::~Application()
    {
        delete window;
    }

    void Application::OnEvent(Event::Event& event)
    {
        Event::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Event::WindowCloseEvent>(
            std::bind(&Application::OnWindowClose, this, std::placeholders::_1)
        );
        dispatcher.Dispatch<Event::WindowResizeEvent>(
            std::bind(&Application::OnWindowResize, this, std::placeholders::_1)
        );
    }

    bool Application::OnWindowClose(Event::Event& event)
    {
        m_running = false;
        return true;
    }

    bool Application::OnWindowResize(Event::Event& event)
    {
        return true;
    }

    void Application::Run()
    {
        while(m_running)
        {
            window->OnUpdate();
        }
    }
} // namespace SekaiEngine
