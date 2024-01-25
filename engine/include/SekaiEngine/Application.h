#ifndef _SEKAI_ENGINE_APPLICATION_H_
#define _SEKAI_ENGINE_APPLICATION_H_

#include "BaseType.h"
#include "SekaiEngine/Window.h"
#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Layer/LayerStack.h"
#include "SekaiEngine/Timer.h"

namespace SekaiEngine
{
    class Application 
    {
    public:
        EXTENDAPI Application();
        EXTENDAPI Application(const Application& app);
        EXTENDAPI virtual ~Application();

        EXTENDAPI void OnEvent(Event::Event& event);

        EXTENDAPI bool OnWindowClose(Event::Event& event);
        EXTENDAPI bool OnWindowResize(Event::Event& event);

        EXTENDAPI void PushLayer(Layer::Layer* layer);
        EXTENDAPI void PushOverlay(Layer::Layer* overlay);
        EXTENDAPI void Run();
        
        EXTENDAPI void loop();
    private:
        IWindow* window;
        bool m_running;
        Timer m_timer;
        Layer::LayerStack m_layerStack;

    };

    //To be defined by CLIENT
    Application* CreateApplication();
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_APPLICATION_H_