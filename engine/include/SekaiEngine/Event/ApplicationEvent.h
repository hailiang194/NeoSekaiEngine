/**
 * @file ApplicationEvent.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the events by application
 * @version 0.1
 * @date 2024-07-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_EVENT_APPLICATION_EVENT_H_
#define _SEKAI_ENGINE_EVENT_APPLICATION_EVENT_H_

#include "./Event.h"
#include "SekaiEngine/Timer.h"

namespace SekaiEngine
{
    namespace Event
    {
        /**
         * @brief Event for application tick
         * 
         */
        class ApplicationTickEvent: public Event
        {
        public:
            /**
             * @brief Construct a new Application Tick Event object
             * 
             */
            ApplicationTickEvent() = default;

            /**
             * @brief Construct a new Application Tick Event object
             * 
             * @param event copied object
             */
            ApplicationTickEvent(const ApplicationTickEvent& event) = default;

            /**
             * @brief Copied assignement operator
             * 
             * @param event copied object
             * @return ApplicationTickEvent& the reference of the object itself 
             */
            ApplicationTickEvent& operator=(const ApplicationTickEvent& event) = default;

            /**
             * @brief Destroy the Application Tick Event object
             * 
             */
            ~ApplicationTickEvent() {}

            EVENT_CLASS_TYPE(SekaiEngine::Event::EventType::AppTick)
            EVENT_CLASS_CATEGORY(EventCategoryApplication)
        };

        /**
         * @brief Event of application update
         * 
         */
        class ApplicationUpdateEvent: public Event
        {
        public:
            /**
             * @brief Construct a new Application Update Event object
             * 
             * @param eplise the eplise time, the time for doing last frame
             */
            ApplicationUpdateEvent(const Timestep& eplise);

            /**
             * @brief Construct a new Application Update Event object
             * 
             * @param event copied object
             */
            ApplicationUpdateEvent(const ApplicationUpdateEvent& event);

            /**
             * @brief Copied assignement operator
             * 
             * @param event copied object
             * @return ApplicationUpdateEvent& the reference of the object itself
             */
            ApplicationUpdateEvent& operator=(const ApplicationUpdateEvent& event);

            /**
             * @brief Destroy the Application Update Event object
             * 
             */
            ~ApplicationUpdateEvent();

            /**
             * @brief Get elipse time, the time for doing in last frame
             * 
             * @return const Timestep& the elipse time
             */
            const Timestep& Elipse() const;

            /**
             * @brief Get elipse time, the time for doing in last frame
             * 
             * @return const Timestep& the elipse time
             */
            const Timestep& Elipse();

            EVENT_CLASS_TYPE(SekaiEngine::Event::EventType::AppUpdate)
            EVENT_CLASS_CATEGORY(EventCategoryApplication)
        private:
            Timestep m_elipse; /*!< elipse time, time of the last frame*/
        };
        

        /**
         * @brief Event of application render
         * 
         */
        class ApplicationRenderEvent: public Event
        {
        public:
            /**
             * @brief Construct a new Application Render Event object
             * 
             */
            ApplicationRenderEvent() = default;

            /**
             * @brief Construct a new Application Render Event object
             * 
             * @param event copied object
             */
            ApplicationRenderEvent(const ApplicationRenderEvent& event) = default;

            /**
             * @brief Copied assignement operator
             * 
             * @param event copied object
             * @return ApplicationRenderEvent& the reference of the object itself
             */
            ApplicationRenderEvent& operator=(const ApplicationRenderEvent& event) = default;

            /**
             * @brief Destroy the Application Render Event object
             * 
             */
            ~ApplicationRenderEvent() {}

            EVENT_CLASS_TYPE(SekaiEngine::Event::EventType::AppRender)
            EVENT_CLASS_CATEGORY(EventCategoryApplication)
        };

        inline const Timestep& ApplicationUpdateEvent::Elipse() const
        {
            return m_elipse;
        }

        inline const Timestep& ApplicationUpdateEvent::Elipse()
        {
            return static_cast<const ApplicationUpdateEvent&>(*this).Elipse();
        }
    } // namespace Event
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_EVENT_APPLICATION_EVENT_H_