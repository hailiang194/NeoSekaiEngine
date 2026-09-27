/**
 * @file Application.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Base Application file, which include the contructor of the whole game
 * @version 0.1
 * @date 2024-07-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_APPLICATION_H_
#define _SEKAI_ENGINE_APPLICATION_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "BaseType.h"
#include "SekaiEngine/Window.h"
#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Layer/LayerStack.h"
#include "SekaiEngine/Render/DrawCmd.h"
#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Audio/Device.h"
#include "SekaiEngine/TextEngine/TextEngine.h"

namespace SekaiEngine
{
    /**
     * @brief Base Application class, which is the container of the whole game
     * 
     */
    class Application 
    {
    public:
        /**
         * @brief Default contructor 
         */
        EXTENDAPI Application();

        /**
         * @brief Copy contructor
         * 
         * @param app  the Application for copy contructor
         */
        EXTENDAPI Application(const Application& app);

        /**
         * @brief Destructor 
         * 
         */
        EXTENDAPI virtual ~Application();

        /**
         * @brief Get the constant reference of Window object of the game
         * 
         * @return const IWindow& The Window object
         */
        EXTENDAPI const IWindow& Window();

        /**
         * @brief Get the reference of AudioDevice
         * 
         * @return Device& Audio device object
         */
        EXTENDAPI SekaiEngine::Audio::Device& AudioDevice();

        EXTENDAPI SekaiEngine::TextEngine::TextEngine& TextEngine();

        /**
         * @brief Handle events
         * 
         * @param event Event needs to handle
         */
        EXTENDAPI void OnEvent(Event::Event& event);

        /**
         * @brief Handle window close
         * 
         * @param event Window Close event
         * @return bool true if the event is handled successfully. Otherwise, false.
         */
        EXTENDAPI bool OnWindowClose(Event::Event& event);

        /**
         * @brief Handle window resize
         * 
         * @param event Window resize event
         * @return bool true if the event is handled successfully. Otherwise, false.
         */
        EXTENDAPI bool OnWindowResize(Event::Event& event);

        /**
         * @brief Handle window is getting focus
         * 
         * @param event Window focus event
         * @return true if the event is handled successfully
         * @return false if the event is not handled successfully
         */
        EXTENDAPI bool OnWindowFocus(Event::Event& event);

        /**
         * @brief Handle window lost focus
         * 
         * @param event Window lost focus event
         * @return true if the event is handled successfully
         * @return false if the event is not handleed successfully
         */
        EXTENDAPI bool OnWindowLostFocus(Event::Event& event);

        /**
         * @brief Handle window move
         * 
         * @param event Window move event
         * @return true if the event is handled successfully
         * @return false if the event is not handled successully
         */
        EXTENDAPI bool OnWindowMove(Event::Event& event);

        /**
         * @brief Handle when application tick
         * 
         * @param event Applcation tick event
         * @return true if the event is handled successfully
         * @return false if the event is not handled successfullt
         */
        EXTENDAPI bool OnApplicationTick(Event::Event& event);

        /**
         * @brief Handle when application update
         * 
         * @param event Application update event
         * @return true if the event is handled successfully
         * @return false if the event is not handled successfully
         */
        EXTENDAPI bool OnApplicationUpdate(Event::Event& event);

        /**
         * @brief Handle when application render
         * 
         * @param event Application render event
         * @return true if the event is handled successfully
         * @return false if the event is not handled successfully
         */
        EXTENDAPI bool OnApplicationRender(Event::Event& event);

        /**
         * @brief Add layer at the background of stack
         * 
         * @param layer added layer
         *
         * @note Refused and reported once Run has started: both threads read the stack
         * every frame, so it cannot be changed while the loop runs.
         */
        EXTENDAPI void PushLayer(Layer::Layer* layer);

        /**
         * @brief Add the layer at the frontgound of stack
         * 
         * @param overlay  added layer
         *
         * @note Refused and reported once Run has started, see PushLayer.
         */
        EXTENDAPI void PushOverlay(Layer::Layer* overlay);

        /**
         * @brief run the game loop
         *
         * @note On desktop this runs the two-thread pipeline: the update thread
         * records a frame's draws while this thread, which owns the graphics context,
         * replays the previous frame. On Web it hands the serial loop to the browser.
         */
        EXTENDAPI void Run();
        
        /**
         * @brief define the loop of the game, update and replay on one thread
         *
         * @note The serial frame. It is what the Web build runs, and it is the
         * reference the threaded pipeline has to match.
         */
        EXTENDAPI void loop();

        /**
         * @brief get the instance of Application
         * 
         * @return Application* the instance of Application
         */
        EXTENDAPI static Application* Instance();
        
    private:
        IWindow* window;
        std::atomic<bool> m_running;
        bool m_loopRunning;
        Timer m_timer;
        Layer::LayerStack m_layerStack;
        SekaiEngine::Audio::Device m_audioDevice;
        SekaiEngine::TextEngine::TextEngine m_textEngine;

        /* Two frame buffers alternating: the update thread records into the one
           main is not replaying, so no buffer is written while it is read. */
        std::vector<Render::DrawCmd> m_frameBuffer[2];
        /* The update thread may record a frame. Taken by the update thread when it
           waits, given back by main once it has taken the frame it just filled. */
        std::atomic<bool> m_permitted;
        /* Index of the buffer holding a finished frame, or -1 when there is none. */
        std::atomic<int> m_filled;
        /* Held only while a thread is idle, and never while a buffer is touched. */
        std::mutex m_park;
        std::condition_variable m_parkSignal;
        std::thread m_updateThread;
        int m_writeSlot;
        Timestep m_nextTimestep;

        static Application* g_instance;

        /**
         * @brief The window's half of a frame: tick event, timer, and the event poll
         *
         * @return Timestep the timestep of the frame, published before the update is permitted
         */
        Timestep BeginFrame();

        /**
         * @brief The update half of a frame, run on whichever thread the frame uses
         *
         * @param elipse timestep of the frame
         *
         * @note The caller owns the recording scope, so the buffer being written is
         * chosen at the call site rather than hidden here.
         */
        void UpdateFrame(const Timestep& elipse);

        /**
         * @brief The update thread's body, for the whole run
         *
         */
        void UpdateLoop();

        /**
         * @brief Refuse any further change to the layer stack, from here on
         *
         */
        void EnterLoop();

        /**
         * @brief Request shutdown and wake the update thread
         *
         */
        void RequestShutdown();
    };

    inline const IWindow& Application::Window()
    {
        return *window;
    }

    inline SekaiEngine::Audio::Device& Application::AudioDevice()
    {
        return m_audioDevice;
    }

    inline SekaiEngine::TextEngine::TextEngine& Application::TextEngine()
    {
        return m_textEngine;
    }

    /**
     * @brief Create a Application object, it is defined by client which means game developer
     * 
     * @return Application* the pointer of the game application
     */
    Application* CreateApplication();
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_APPLICATION_H_