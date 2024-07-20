/**
 * @file Window.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Window class for Raylib library
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_WRAPPER_RAYLIB_WINDOW_H_
#define _SEKAI_ENGINE_WRAPPER_RAYLIB_WINDOW_H_

#ifdef USE_RAYLIB

#include "SekaiEngine/Window.h"
#include "SekaiEngine/Math/Vector.h"

namespace Wrapper
{
    namespace Raylib
    {
        class Window: public SekaiEngine::IWindow
        {
        public:
            Window(const SekaiEngine::WindowsProps& props);
            Window(const Window& window);
            Window& operator=(const Window& window);
            virtual ~Window();  

            void OnUpdate() override;
            int GetHeight() const override;
            int GetWidth() const override;
            void setEventCallbackFn(const EventCallbackFn& fn) override;

            void SetVSync(bool enable) override;
            bool IsVSync() const override;
        private:
            unsigned int m_flag;
            IWindow::EventCallbackFn m_eventCallbackFn;
            bool m_isFocus;
            SekaiEngine::Math::Vector2D m_windowPosition;

            void _pollEvent();
        };

        inline void Window::setEventCallbackFn(const EventCallbackFn& fn)
        {
            m_eventCallbackFn = fn;
        }
    } // namespace Raylib
    
} // namespace Wrapper

#endif//USE_RAYLIB

#endif //!_SEKAI_ENGINE_WRAPPER_RAYLIB_WINDOW_H_