/**
 * @file EntryPoint.h
 * @author hailiang194 (hailuongthe2000@gmail.com)
 * @brief Entry point, which is the main function, of the game
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_ENTRY_POINT_H_
#define _SEKAI_ENGINE_ENTRY_POINT_H_

/**
 * \def HAS_ENTRY_POINT
 * This macro is defined in the App part, which self-implement the main function
 * 
 */

#ifdef HAS_ENTRY_POINT

extern SekaiEngine::Application* SekaiEngine::CreateApplication();

int main(int argc, char**argv)
{
    auto app = SekaiEngine::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif

#endif//!_SEKAI_ENGINE_ENTRY_POINT_H_