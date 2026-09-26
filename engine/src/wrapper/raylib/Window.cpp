#include "wrapper/raylib/Window.h"
#include "SekaiEngine/Event/WindowEvent.h"

#ifdef USE_RAYLIB
#include "raylib.h"

namespace Wrapper
{
    namespace Raylib
    {
        Window::Window(const SekaiEngine::WindowsProps& props)
            :m_flag(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT), m_isFocus(true), m_windowPosition()
        {
            SetConfigFlags(m_flag);
            InitWindow(props.Width, props.Height, props.Title);
        }

        Window::Window(const Window& window)
            :m_flag(window.m_flag), m_isFocus(window.m_isFocus), m_windowPosition(window.m_windowPosition)
        {

        }

        Window& Window::operator=(const Window& window)
        {
            m_flag = window.m_flag;
            m_isFocus = window.m_isFocus;
            m_windowPosition = window.m_windowPosition;

            return (*this);
        }

        Window::~Window()
        {
            CloseWindow();
        }  

        void Window::OnUpdate()
        {
            _pollEvent();
        }

        int Window::GetHeight() const
        {
            return GetScreenHeight();
        }

        int Window::GetWidth() const
        {
            return GetScreenWidth();
        }

        void Window::SetVSync(bool enable)
        {
            if(enable)
            {
                SetWindowState(FLAG_VSYNC_HINT);
            }
            else
            {
                ClearWindowState(FLAG_VSYNC_HINT);
            }
        }

        bool Window::IsVSync() const
        {
            return IsWindowState(FLAG_VSYNC_HINT);
        }

        void Window::_pollEvent()
        {
            if(WindowShouldClose())
            {
                SekaiEngine::Event::WindowCloseEvent event;
                m_eventCallbackFn(event);
            }
            if(IsWindowResized())
            {
                SekaiEngine::Event::WindowResizeEvent event(GetWidth(), GetHeight());
                m_eventCallbackFn(event);
            }
            if(!m_isFocus && IsWindowFocused())
            {
                m_isFocus = true;
                SekaiEngine::Event::WindowFocusEvent event;
                m_eventCallbackFn(event);
            }
            if(m_isFocus && !IsWindowFocused())
            {
                m_isFocus = false;
                SekaiEngine::Event::WindowLostFocusEvent event;
                m_eventCallbackFn(event);
            }
            
            ::Vector2 rWinPos = ::GetWindowPosition();
            SekaiEngine::Math::Vector2D winPos(rWinPos.x, rWinPos.y);
            if(!(winPos == m_windowPosition))
            {
                SekaiEngine::Event::WindowMovedEvent event;
                m_eventCallbackFn(event);
                m_windowPosition = winPos;
            }
        }

    } // namespace Raylib
    
} // namespace Wrapper


#endif