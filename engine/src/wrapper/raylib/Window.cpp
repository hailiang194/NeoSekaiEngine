#include "wrapper/raylib/Window.h"

#ifdef USE_RAYLIB
#include "raylib.h"

namespace Wrapper
{
    namespace Raylib
    {
        Window::Window(const SekaiEngine::WindowsProps& props)
            :m_flag(0)
        {
            InitWindow(props.Width, props.Height, props.Title);
            SetVSync(true);
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

    } // namespace Raylib
    
} // namespace Wrapper


#endif