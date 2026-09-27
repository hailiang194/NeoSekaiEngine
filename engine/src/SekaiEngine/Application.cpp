#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Event/WindowEvent.h"
#include "SekaiEngine/Event/ApplicationEvent.h"
#include "SekaiEngine/Application.h"
#include "SekaiEngine/Render/Renderer.h"
#include "SekaiEngine/Render/RenderCommand.h"
#include "SekaiEngine/Input.h"
#include "SekaiEngine/Render/Texture.h"
#include "SekaiEngine/Render/Font.h"
#include "SekaiEngine/TextEngine/TextEngine.h"
#include "SekaiEngine/Audio/Sound.h"
#include "SekaiEngine/Audio/MusicStream.h"
#include <iostream>
#include "version.h"

namespace SekaiEngine
{
    Application* Application::g_instance = nullptr;

    Application::Application()
        :window(IWindow::Create()), m_audioDevice(), m_running(true), m_loopRunning(false), m_layerStack(), m_timer(),
         m_permitted(false), m_filled(-1), m_writeSlot(0)
    {
        std::cout << "You are using NeoSekaiEngine v" << SEKAI_ENGINE_VERSION << std::endl;
        Application::g_instance = this;
        window->setEventCallbackFn(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        SekaiEngine::Render::initTextures();
        SekaiEngine::Render::initFonts();
        SekaiEngine::TextEngine::initTextEngine();
        SekaiEngine::Sound::initSounds();
        SekaiEngine::Sound::initMusicStreams();
        m_timer.SetTargetFPS(60);
    }
    Application::Application(const Application& app)
        :window(app.window), m_running(app.m_running.load()), m_loopRunning(false), m_layerStack(app.m_layerStack), m_timer(), m_textEngine(),
         m_permitted(false), m_filled(-1), m_writeSlot(0)
    {

    }

    Application::~Application()
    {
#if !defined(USE_RAYLIB) || defined(PLATFORM_WEB)
#else
        //Run joins before returning, so this is the backstop that stops teardown
        //unloading the registries while a frame is still being recorded.
        if(m_updateThread.joinable())
        {
            m_updateThread.join();
        }
#endif
        SekaiEngine::Sound::unloadMusicStreams();
        SekaiEngine::Sound::unloadSounds();
        SekaiEngine::Render::unloadFonts();
        SekaiEngine::Render::destroyTextures();
        SekaiEngine::TextEngine::unloadTextEngine();
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
        dispatcher.Dispatch<Event::WindowFocusEvent>(
            std::bind(&Application::OnWindowFocus, this, std::placeholders::_1)
        );
        dispatcher.Dispatch<Event::WindowLostFocusEvent>(
            std::bind(&Application::OnWindowLostFocus, this, std::placeholders::_1)
        );
        dispatcher.Dispatch<Event::WindowMovedEvent>(
            std::bind(&Application::OnWindowMove, this, std::placeholders::_1)
        );

        dispatcher.Dispatch<Event::ApplicationTickEvent>(
            std::bind(&Application::OnApplicationTick, this, std::placeholders::_1)
        );

        dispatcher.Dispatch<Event::ApplicationUpdateEvent>(
            std::bind(&Application::OnApplicationUpdate, this, std::placeholders::_1)
        );

        dispatcher.Dispatch<Event::ApplicationRenderEvent>(
            std::bind(&Application::OnApplicationRender, this, std::placeholders::_1)
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
        RequestShutdown();
        return true;
    }

    void Application::RequestShutdown()
    {
#if !defined(USE_RAYLIB) || defined(PLATFORM_WEB)
        m_running = false;
#else
        {
            //The notify has to happen under the lock the update thread parks on:
            //Run breaks out of its loop on the close path without ever releasing the
            //permit, so a thread already parked in wait would otherwise never wake and
            //the join at the end of Run would hang the process at exit.
            std::lock_guard<std::mutex> lock(m_park);
            m_running = false;
        }
        m_parkSignal.notify_all();
#endif
    }

    bool Application::OnWindowResize(Event::Event& event)
    {
        Event::WindowResizeEvent& resizeEvt = dynamic_cast<Event::WindowResizeEvent&>(event);
        Render::Renderer::OnWindowResize(resizeEvt.Width(), resizeEvt.Height());
        return true;
    }

    bool Application::OnWindowFocus(Event::Event& event)
    {
        return true;
    }

    bool Application::OnWindowLostFocus(Event::Event& event)
    {
        return true;
    }

    bool Application::OnWindowMove(Event::Event& event)
    {
        return true;
    }

    bool Application::OnApplicationTick(Event::Event& event)
    {
        return true;
    }

    bool Application::OnApplicationUpdate(Event::Event& event)
    {
        Sound::updateMusicStream();
        return true;
    }

    bool Application::OnApplicationRender(Event::Event& event)
    {
        return true;
    }

    void Application::EnterLoop()
    {
        m_loopRunning = true;
    }

    void Application::PushLayer(Layer::Layer* layer)
    {
        if(m_loopRunning)
        {
            std::cerr << "SekaiEngine: refused to add a layer while the loop is running. "
                "Both threads read the layer stack every frame, so it cannot be changed "
                "after Run has started" << std::endl;
            return;
        }
        m_layerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer::Layer* overlay)
    {
        if(m_loopRunning)
        {
            std::cerr << "SekaiEngine: refused to add an overlay while the loop is running. "
                "Both threads read the layer stack every frame, so it cannot be changed "
                "after Run has started" << std::endl;
            return;
        }
        m_layerStack.PushOverlay(overlay);
    }   

#if !defined(USE_RAYLIB) || defined(PLATFORM_WEB)
#else
    void Application::Run()
    {
        EnterLoop();
        m_writeSlot = 0;
        m_filled.store(-1);
        //Seed the first frame's timestep and the first permit, so the update thread
        //has something to record before BeginFrame has ever run.
        m_nextTimestep = m_timer.update();
        m_permitted.store(true, std::memory_order_release);
        m_updateThread = std::thread(&Application::UpdateLoop, this);

        while(m_running)
        {
            {
                std::unique_lock<std::mutex> lock(m_park);
                m_parkSignal.wait(lock, [this] { return m_filled.load() >= 0 || !m_running; });
                if(!m_running)
                    break;
            }

            m_nextTimestep = BeginFrame();

            int slot = m_filled.exchange(-1, std::memory_order_acquire);
            /*The permit must be released after BeginFrame, never before it.
              BeginFrame dispatches the tick event and the window poll, and OnEvent
              walks the layer stack calling Layer::OnEvent, so that dispatch reaches
              layer code on this thread only while the update thread is still parked.
              Releasing the permit above BeginFrame would put Layer::OnEvent and
              Layer::OnUpdate on the same layer at the same time.*/
            m_permitted.store(true, std::memory_order_release);
            m_parkSignal.notify_all();

            //The update thread now records the other buffer for the whole replay.
            SekaiEngine::Render::RenderCommand::ReplayFrame(m_frameBuffer[slot], (SekaiEngine::Render::Color)0x000000ff);
            m_frameBuffer[slot].clear();
        }
        m_updateThread.join();
    }

    void Application::UpdateLoop()
    {
        while(m_running)
        {
            {
                std::unique_lock<std::mutex> lock(m_park);
                m_parkSignal.wait(lock, [this] { return m_permitted.load() || !m_running; });
                if(!m_running)
                    break;
                /*Spend the permit, under the lock, so it cannot be given back and
                  taken away again between the predicate above and here. Without it the
                  permit is only a level and this thread would record frames back to
                  back, overwriting the buffer the other thread is replaying.*/
                m_permitted.store(false, std::memory_order_relaxed);
            }

            std::vector<Render::DrawCmd>& buffer = m_frameBuffer[m_writeSlot];
            buffer.clear();
            SekaiEngine::Render::RenderCommand::BeginRecording(buffer);
            UpdateFrame(m_nextTimestep);
            SekaiEngine::Render::RenderCommand::EndRecording();

            //This release and the exchange in Run are the only synchronisation the
            //recorded draws themselves depend on. No lock is held across either.
            m_filled.store(m_writeSlot, std::memory_order_release);
            m_writeSlot = 1 - m_writeSlot;
            m_parkSignal.notify_all();
        }
    }
#endif

    Timestep Application::BeginFrame()
    {
        Event::ApplicationTickEvent tickEvent;
        OnEvent(tickEvent);
        /*The timestep covers the previous frame's submission plus the wait for the
          update thread to finish the frame it was last given the permit for, which is
          the same interval the serial loop measures. Measuring it on the update thread
          would leave the submission cost out of every delta and run every animation slow.*/
        Timestep elipse = m_timer.update();
        window->OnUpdate();
        return elipse;
    }

    void Application::UpdateFrame(const Timestep& elipse)
    {
        Event::ApplicationUpdateEvent updateEvent(elipse);
        OnEvent(updateEvent);
        for(auto it = m_layerStack.begin(); it != m_layerStack.end(); ++it)
        {
            (*it)->OnUpdate(elipse);
        }

        Event::ApplicationRenderEvent renderEvent;
        OnEvent(renderEvent);
        for(auto it = m_layerStack.begin(); it != m_layerStack.end(); ++it)
        {
            (*it)->OnRender();
        }
    }

    void Application::loop()
    {
        Timestep elipse = BeginFrame();
        if(!m_running)
            return;

        std::vector<SekaiEngine::Render::DrawCmd> frameBuffer;
        SekaiEngine::Render::RenderCommand::BeginRecording(frameBuffer);
        UpdateFrame(elipse);
        SekaiEngine::Render::RenderCommand::EndRecording();

        SekaiEngine::Render::RenderCommand::ReplayFrame(frameBuffer, (SekaiEngine::Render::Color)0x000000ff);
    }



    Application* Application::Instance()
    {
        return g_instance;
    }
} // namespace SekaiEngine
