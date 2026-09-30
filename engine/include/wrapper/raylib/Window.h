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
        /**
         * @brief Raylib implementation of the engine window
         *
         * Owns the raylib window, so only one may exist at a time and it must
         * be destroyed after everything that draws.
         */
        class Window: public SekaiEngine::IWindow
        {
        public:
            /**
             * @brief Construct a new Window object and open the raylib window
             *
             * The window is created resizable with VSync hinted, and the
             * raylib log bridge is installed first so that the warnings raised
             * during initialization are not lost.
             *
             * @param props the size, title and other properties of the window
             */
            Window(const SekaiEngine::WindowsProps& props);

            /**
             * @brief Construct a new Window object
             *
             * @param window copied object
             */
            Window(const Window& window);

            /**
             * @brief Copied assignment operator
             *
             * @param window the object copy from
             * @return Window& the reference of the object itself
             */
            Window& operator=(const Window& window);

            /**
             * @brief Destroy the Window object and close the raylib window
             *
             */
            virtual ~Window();

            /**
             * @brief Poll the window events and dispatch them to the callback
             *
             * @note called once per frame by the engine
             */
            void OnUpdate() override;

            /**
             * @brief Get the height of the window
             *
             * @return int the height in pixels
             */
            int GetHeight() const override;

            /**
             * @brief Get the width of the window
             *
             * @return int the width in pixels
             */
            int GetWidth() const override;

            /**
             * @brief Set the function called with every window event
             *
             * @param fn the callback function
             */
            void setEventCallbackFn(const EventCallbackFn& fn) override;

            /**
             * @brief Enable or disable VSync
             *
             * @param enable true to sync the frame rate with the display refresh rate
             */
            void SetVSync(bool enable) override;

            /**
             * @brief Check if VSync is enabled
             *
             * @return true VSync is enabled
             * @return false VSync is disabled
             */
            bool IsVSync() const override;
        private:
            unsigned int m_flag; /*!< the raylib config flags the window was created with */
            IWindow::EventCallbackFn m_eventCallbackFn; /*!< the function every window event is dispatched to */
            bool m_isFocus; /*!< whether the window is focused, tracked to emit focus events once per change */
            SekaiEngine::Math::Vector2D m_windowPosition; /*!< the last known window position, tracked to emit move events */

            /**
             * @brief Poll the raylib window state and emit the matching events
             *
             */
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