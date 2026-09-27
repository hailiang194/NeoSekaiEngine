#include "SekaiEngine/Application.h"

#if defined(USE_RAYLIB) && defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
namespace SekaiEngine
{
    void RunLoop(void* app)
    {
        reinterpret_cast<Application*>(app)->loop();
    }

    void Application::Run()
    {
        //Arm the layer stack refusal here too, not only in the desktop pipeline:
        //this is the Run the Web build runs, and leaving it out would make the rule
        //hold on desktop and silently not hold on Web.
        EnterLoop();
        emscripten_set_main_loop_arg(RunLoop, (void*)this, 0, 1);
    }
} // namespace SekaiEngine

#endif