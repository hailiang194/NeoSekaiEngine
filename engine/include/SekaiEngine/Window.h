/**
 * @file Window.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Window interface
 * @version 0.1
 * @date 2024-07-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_WINDOW_H_
#define _SEKAI_ENGINE_WINDOW_H_

#include <functional>
#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Event/Event.h"

namespace SekaiEngine 
{
    /**
     * @brief The properties of Windows
     * 
     */
    class EXTENDAPI WindowsProps
    {
    public:
        /**
         * @brief Construct a new Windows Props object
         * 
         * @param title the title of main window
         * @param width the width of main window
         * @param height the height of main window
         */
        WindowsProps(const char* title = "Sekai Engine", const int& width = 1680, const int& height = 720);

        /**
         * @brief Construct a new Windows Props object
         * 
         * @param props copied object
         */
        WindowsProps(const WindowsProps& props);

        /**
         * @brief Copy assignment operator of WindowsPros
         * 
         * @param props copied object
         * @return WindowsProps& the reference of itself
         */
        WindowsProps& operator=(const WindowsProps& props);

        /**
         * @brief Destroy the Windows Props object
         * 
         */
        virtual ~WindowsProps();

        const char* Title; /*!< the title of game*/
        int Height; /*!< the window height*/
        int Width; /*!< the width width*/
    };

    /**
     * @brief Window interface
     * 
     */
    class EXTENDAPI IWindow
    {
    public:
        /**
         * @brief Callback event function type
         * 
         */
        using EventCallbackFn = std::function<void(Event::Event&)>;

        /**
         * @brief Construct a new IWindow object
         * 
         */
        IWindow() = default;

        /**
         * @brief Construct a new IWindow object
         * 
         * @param window copied object
         */
        IWindow(const IWindow& window) = default;

        /**
         * @brief Copy assignment operator
         * 
         * @param window copied object
         * @return IWindow& the reference of itself
         */
        IWindow& operator=(const IWindow& window) = default;

        /**
         * @brief Destroy the IWindow object
         * 
         */
        virtual ~IWindow();

        /**
         * @brief Handle when run a loop
         * 
         */
        virtual void OnUpdate() = 0;

        /**
         * @brief Get the height of window
         * 
         * @return int the height of window
         */
        virtual int GetHeight() const = 0;

        /**
         * @brief Get the height of window
         * 
         * @return int the height of window
         */
        int GetHeight();

        /**
         * @brief Get the width of window
         * 
         * @return int the width of window
         */
        virtual int GetWidth() const = 0;

        /**
         * @brief Get the width of window
         * 
         * @return int the width of window
         */
        int GetWidth();

        /**
         * @brief Set the Event Callback Fn object
         * 
         * @param fn event callback function
         */
        virtual void setEventCallbackFn(const EventCallbackFn& fn) = 0;

        /**
         * @brief Set VSync value
         * 
         * @param enable the enable value of VSync
         */
        virtual void SetVSync(bool enable) = 0;

        /**
         * @brief check if VSync is enable
         * 
         * @return true if VSync is enabled
         * @return false if VSync is disabled
         */
        virtual bool IsVSync() const = 0;

        /**
         * @brief create window
         * 
         * @param props window properties
         * @return IWindow* created window
         */
        static IWindow* Create(const WindowsProps& props = WindowsProps());
    };

    inline int IWindow::GetHeight()
    {
        return static_cast<const IWindow&>(*this).GetHeight();
    }

    inline int IWindow::GetWidth()
    {
        return static_cast<const IWindow&>(*this).GetWidth();
    }
}

#endif//!_SEKAI_ENGINE_WINDOW_H_