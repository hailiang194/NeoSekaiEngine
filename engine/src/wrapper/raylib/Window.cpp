#include "wrapper/raylib/Window.h"
#include "SekaiEngine/Event/WindowEvent.h"

#ifdef USE_RAYLIB
#include "raylib.h"

namespace Wrapper
{
    namespace Raylib
    {
        Window::Window(const SekaiEngine::WindowsProps& props)
            :m_flag(FLAG_WINDOW_RESIZABLE)
        {
            SetVSync(true);
            InitWindow(props.Width, props.Height, props.Title);
            SetTargetFPS(60);
        }

        Window::Window(const Window& window)
            :m_flag(window.m_flag)
        {

        }

        Window& Window::operator=(const Window& window)
        {
            m_flag = window.m_flag;
            return (*this);
        }

        Window::~Window()
        {
            CloseWindow();
        }  

        void Window::OnUpdate()
        {
            _pollEvent();
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        int Window::GetHeight()
        {
            return GetScreenHeight();
        }

        int Window::GetWidth()
        {
            return GetScreenWidth();
        }

        void Window::SetVSync(bool enable)
        {
            if(!enable && IsVSync())
            {
                m_flag -= FLAG_VSYNC_HINT;
            }
            else if(enable && !IsVSync())
            {
                m_flag |= FLAG_VSYNC_HINT;
            }

            SetConfigFlags(m_flag);
        }

        bool Window::IsVSync() const
        {
            return m_flag & FLAG_VSYNC_HINT;
        }

        void Window::_pollEvent()
        {
            if(WindowShouldClose())
            {
                m_eventCallbackFn(SekaiEngine::Event::WindowCloseEvent());
            }
            if(IsWindowResized())
            {
                m_eventCallbackFn(SekaiEngine::Event::WindowResizeEvent(GetWidth(), GetHeight()));
            }
        }

    } // namespace Raylib
    
} // namespace Wrapper


#endif