#ifndef _SEKAI_ENGINE_APPLICATION_H_
#define _SEKAI_ENGINE_APPLICATION_H_

#include "BaseType.h"

namespace SekaiEngine
{
    class EXTENDAPI Application 
    {
    public:
        Application();
        Application(const Application& app);
        virtual ~Application();

        void Run();
    };

    //To be defined by CLIENT
    Application* CreateApplication();
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_APPLICATION_H_