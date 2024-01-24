#ifndef _SEKAI_ENGINE_APPLICATION_H_
#define _SEKAI_ENGINE_APPLICATION_H_

#include "BaseType.h"
#include "SekaiEngine/Window.h"
#include "SekaiEngine/Event/Event.h"

namespace SekaiEngine
{
    class EXTENDAPI Application 
    {
    public:
        Application();
        Application(const Application& app);
        virtual ~Application();

        void OnEvent(Event::Event& event);

        bool OnWindowClose(Event::Event& event);
        bool OnWindowResize(Event::Event& event);
        void Run();
    private:
        IWindow* window;
        bool m_running;
    };

    //To be defined by CLIENT
    Application* CreateApplication();
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_APPLICATION_H_