/**
 * @file Event.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Create and handle event
 * @version 0.1
 * @date 2024-07-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_EVENT_EVENT_H_
#define _SEKAI_ENGINE_EVENT_EVENT_H_

#include <functional>
#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Event
    {
        /**
         * @brief Event type
         * 
         */
        enum class EventType
        {
            None = 0, /*!< None*/
            WindowClose, /*!< Close window*/
            WindowResize, /*!< Resize Window*/
            WindowFocus, /*!< Window in focus*/
            WindowLostFocus, /*!< Window lost focus*/
            WindowMoved, /*!< Window move*/
            AppTick, /*!< App tick*/
            AppUpdate,  /*!< App update*/
            AppRender, /*!< App render*/
            KeyPressed, /*!< Key is pressed*/
            KeyRelease, /*!< Key is release*/
            MouseButtonPressed, /*!< Mouse button is pressed*/
            MouseButtonRelease, /*!< Mouse button is release*/
            MouseMoved, /*!< Mouse button is moved*/
            MouseScrolled /*!< Mouse button is scrolled*/
        };

        /**
         * @brief Event category
         * 
         */
        enum EventCategory
        {
            None = 0, /*!< None or unknown category*/
            EventCategoryApplication = 0x1, /*!< Event from the application*/
            EventCategoryInput = 0x2, /*!< Event from the input*/
            EventCategoryKeyboard = 0x4, /*!< Event from keyboard*/
            EventCategoryMouse = 0x8, /*!< Event from mouse*/
            EventCategoryMouseButton = 0xf /*!< Event from mouse button*/
        };

/**
 * \def EVENT_CLASS_TYPE(type)  
 * Macro for getting the type of event
 * 
 */
#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return type; } \
                                virtual EventType GetEventType() const override { return GetStaticType(); } \
                                virtual const char* GetName() const override { return #type; }

/**
 * \def EVENT_CLASS_CATEGORY(category) 
 * Macro for getting event category
 * 
 */
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

        /**
         * @brief Event base class
         * 
         */
        class EXTENDAPI Event
        {
        public:
            /**
             * @brief Construct a new Event object
             * 
             */
            Event();

            /**
             * @brief Construct a new Event object
             * 
             * @param event copied event
             */
            Event(const Event& event);

            /**
             * @brief Copy assignment operator
             * 
             * @param event copied object
             * @return Event& the reference of itself
             */
            Event& operator=(const Event& event);

            /**
             * @brief Destroy the Event object
             * 
             */
            virtual ~Event();

            /**
             * @brief Get the Event Type of event
             * 
             * @return EventType the type of event
             */
            virtual EventType GetEventType() const = 0;

            /**
             * @brief Get the Name of event
             * 
             * @return const char* the pointer of name
             */
            virtual const char* GetName() const = 0;

            /**
             * @brief Get the Category Flags of event
             * 
             * @return int the category flag of event
             */
            virtual int GetCategoryFlags() const = 0;

            /**
             * @brief Check if event is in the specific category
             * 
             * @param category event category
             * @return true if the event is in
             * @return false if the event is not in
             */
            bool IsInCategory(EventCategory category);

            bool Handled; /*!< The boolean to check if the event is handled*/
        };

        /**
         * @brief The class is used to dispatch event
         * 
         */
        class EventDispatcher
        {
            template<typename T>
            using EventFn = std::function<bool(T&)>;
        public:
            /**
             * @brief Construct a new Event Dispatcher object
             * 
             * @param event dispatched event
             */
            EventDispatcher(Event& event);

            /**
             * @brief Construct a new Event Dispatcher object
             * 
             * @param dispatcher copied object
             */
            EventDispatcher(const EventDispatcher& dispatcher);

            /**
             * @brief Copy assignment object
             * 
             * @param dispatcher copied object
             * @return EventDispatcher& the reference of itself
             */
            EventDispatcher& operator=(const EventDispatcher& dispatcher);

            /**
             * @brief Destroy the Event Dispatcher object
             * 
             */
            ~EventDispatcher();

            /**
             * @brief dispatch the object
             * 
             * @tparam T Event type
             * @tparam F dispatch function type
             * @param func dispatch function
             * @return true if event is handled
             * @return false if event is not handled
             */
            template<typename T, typename F>
            bool Dispatch(const F& func);

        private:
            Event& m_event;
        };

        inline bool Event::IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

        template<typename T, typename F>
        inline bool EventDispatcher::Dispatch(const F& func)
        {
            if(m_event.GetEventType() == T::GetStaticType())
            {
                m_event.Handled |= func(static_cast<T&>(m_event));
                return true;
            }

            return false;
        }
    } // namespace Event
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_EVENT_EVENT_H_