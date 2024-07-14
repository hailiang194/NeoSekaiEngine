/**
 * @file WindowEvent.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief This header file includes all the event related to window
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_EVENT_WINDOW_H_
#define _SEKAI_ENGINE_EVENT_WINDOW_H_

#include "SekaiEngine/Event/Event.h"

namespace SekaiEngine
{
    namespace Event
    {
        /**
         * @brief Event of window resize
         * 
         */
        class WindowResizeEvent: public Event
        {
        public:
            /**
             * @brief Construct a new Window Resize Event object
             * 
             * @param width new width
             * @param height new height
             */
            WindowResizeEvent(const int& width, const int& height);

            /**
             * @brief Construct a new Window Resize Event object
             * 
             * @param event copied object
             */
            WindowResizeEvent(const WindowResizeEvent& event);

            /**
             * @brief Copied operator assignment
             * 
             * @param event copied object
             * @return WindowResizeEvent& the reference of inself 
             */
            WindowResizeEvent& operator=(const WindowResizeEvent& event);

            /**
             * @brief Destroy the Window Resize Event object
             * 
             */
            virtual ~WindowResizeEvent();

            EVENT_CLASS_TYPE(SekaiEngine::Event::EventType::WindowResize)
            EVENT_CLASS_CATEGORY(EventCategoryApplication)

            /**
             * @brief get the new width of window
             * 
             * @return const int& the width of window
             */
            const int& Width() const;

            /**
             * @brief get the new width of window
             * 
             * @return const int& the width of window
             */
            const int& Width();

            /**
             * @brief get the new height of window
             * 
             * @return const int& the height of window
             */
            const int& Height() const;

            /**
             * @brief get the new height of window
             * 
             * @return const int& the height of window
             */
            const int& Height();

        protected:
            int m_width; /*!< width of window*/
            int m_height; /*!< height of window*/
        }; 

        /**
         * @brief Event of window closing
         * 
         */
        class WindowCloseEvent: public Event
        {
        public:
            /**
             * @brief Construct a new Window Close Event object
             * 
             */
            WindowCloseEvent() = default;

            /**
             * @brief Construct a new Window Close Event object
             * 
             * @param event copied event
             */
            WindowCloseEvent(const WindowCloseEvent& event) = default;

            /**
             * @brief Copied assignment opeator
             * 
             * @param event copied object
             * @return WindowCloseEvent& the reference of itself
             */
            WindowCloseEvent& operator=(const WindowCloseEvent& event) = default;

            /**
             * @brief Destroy the Window Close Event object
             * 
             */
            ~WindowCloseEvent() {}

            EVENT_CLASS_TYPE(SekaiEngine::Event::EventType::WindowClose)
            EVENT_CLASS_CATEGORY(EventCategoryApplication)
        };

        inline const int& WindowResizeEvent::Width() const
        {
            return m_width;
        }

        inline const int& WindowResizeEvent::Width()
        {
            return static_cast<const WindowResizeEvent&>(*this).Width();
        }

        inline const int& WindowResizeEvent::Height() const
        {
            return m_height;
        }

        inline const int& WindowResizeEvent::Height()
        {
            return static_cast<const WindowResizeEvent&>(*this).Height();
        }
    } // namespace Event
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_EVENT_WINDOW_H_