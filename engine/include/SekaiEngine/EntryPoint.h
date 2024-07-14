/**
 * @file EntryPoint.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Entry point, which is the main function, of the game
 * @version 0.1
 * @date 2024-07-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_ENTRY_POINT_H_
#define _SEKAI_ENGINE_ENTRY_POINT_H_

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