#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Event/WindowEvent.h"
#include "SekaiEngine/Application.h"
#include "SekaiEngine/Render/Renderer.h"
#include "SekaiEngine/Render/RenderCommand.h"
#include "SekaiEngine/Input.h"

namespace SekaiEngine
{
    Application::Application()
        :window(IWindow::Create()), m_running(true), m_layerStack()
    {
        window->setEventCallbackFn(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    }
    Application::Application(const Application& app)
        :window(app.window), m_running(app.m_running), m_layerStack(app.m_layerStack)
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

        for(auto it = m_layerStack.end(); it != m_layerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if(event.Handled)
                break;
        }
    }

    bool Application::OnWindowClose(Event::Event& event)
    {
        m_running = false;
        return true;
    }

    bool Application::OnWindowResize(Event::Event& event)
    {
        Event::WindowResizeEvent& resizeEvt = dynamic_cast<Event::WindowResizeEvent&>(event);
        Render::Renderer::OnWindowResize(resizeEvt.Width(), resizeEvt.Height());
        return false;
    }

    void Application::PushLayer(Layer::Layer* layer)
    {
        m_layerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer::Layer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
    }   

#if !defined(USE_RAYLIB) || defined(PLATFORM_WEB)
#else
    void Application::Run()
    {
        while(m_running)
        {
            loop();
        }
    }
#endif

    void Application::loop()
    {
        static SekaiEngine::Render::Camera2D camera;
        Render::RenderCommand::SetClearColor((Render::Color)0xffffffff);
        Render::RenderCommand::Clear();

        Render::Renderer::BeginScreen(camera);

        Render::Renderer::Render();

        Render::Renderer::EndScreen(camera);

        for(auto it = m_layerStack.begin(); it != m_layerStack.end(); ++it)
        {
            (*it)->OnUpdate();
        }
        window->OnUpdate();
    }
} // namespace SekaiEngine
