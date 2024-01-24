#include "SekaiEngine/Application.h"

#if defined(USE_RAYLIB) && defined(PLATFORM_WEB)
namespace SekaiEngine
{
    void RunLoop(Application* app)
    {
        app->loop();
    }

    void Application::Run()
    {
        emscripten_set_main_loop_arg(RunLoop, (void*)this, 0, 1);
    }
} // namespace SekaiEngine

#endif