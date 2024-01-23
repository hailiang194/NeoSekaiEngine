#ifndef _SEKAI_ENGINE_WINDOW_H_
#define _SEKAI_ENGINE_WINDOW_H_

#include <string>
#include "SekaiEngine/BaseType.h"

namespace SekaiEngine 
{
    class EXTENDAPI WindowsProps
    {
    public:
        WindowsProps(const char* title = "Sekai Engine", const int& width = 1680, const int& height = 720);
        WindowsProps(const WindowsProps& props);
        WindowsProps& operator=(const WindowsProps& props);
        virtual ~WindowsProps();

        const char* Title;
        int Height;
        int Width;
    };

    class EXTENDAPI IWindow
    {
    public:
        IWindow() = default;
        IWindow(const IWindow& window) = default;
        IWindow& operator=(const IWindow& window) = default;
        virtual ~IWindow();

        virtual void OnUpdate() = 0;
        virtual int GetHeight() = 0;
        virtual int GetWidth() = 0;

        virtual void SetVSync(bool enable) = 0;
        virtual bool IsVSync() const = 0;

        static IWindow* Create(const WindowsProps& props = WindowsProps());
    };
}

#endif//!_SEKAI_ENGINE_WINDOW_H_