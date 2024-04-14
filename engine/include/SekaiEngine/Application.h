/**
 * @file Application.h
 * @author hailiang194 (hailuongthe2000@gmail.com)
 * @brief Base Application file, which include the contructor of the whole game
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_APPLICATION_H_
#define _SEKAI_ENGINE_APPLICATION_H_

#include "BaseType.h"
#include "SekaiEngine/Window.h"
#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Layer/LayerStack.h"
#include "SekaiEngine/Timer.h"

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
         * @brief Add layer at the background of stack
         * 
         * @param layer added layer
         */
        EXTENDAPI void PushLayer(Layer::Layer* layer);

        /**
         * @brief Add the layer at the frontgound of stack
         * 
         * @param overlay  added layer
         */
        EXTENDAPI void PushOverlay(Layer::Layer* overlay);

        /**
         * @brief run the game loop
         * 
         */
        EXTENDAPI void Run();
        
        /**
         * @brief define the loop of the game
         * 
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
        bool m_running;
        Timer m_timer;
        Layer::LayerStack m_layerStack;

        static Application* g_instance;
    };

    inline const IWindow& Application::Window()
    {
        return *window;
    }

    //To be defined by CLIENT
    Application* CreateApplication();
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_APPLICATION_H_