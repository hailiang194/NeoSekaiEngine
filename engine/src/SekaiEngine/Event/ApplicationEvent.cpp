#include "SekaiEngine/Event/ApplicationEvent.h"

namespace SekaiEngine
{
    namespace Event
    {
            ApplicationUpdateEvent::ApplicationUpdateEvent(const Timestep& elipse)
                :m_elipse(elipse)
            {

            }

            ApplicationUpdateEvent::ApplicationUpdateEvent(const ApplicationUpdateEvent& event)
                :m_elipse(event.m_elipse)
            {

            }

            ApplicationUpdateEvent& ApplicationUpdateEvent::operator=(const ApplicationUpdateEvent& event)
            {
                m_elipse = event.m_elipse;
                return (*this);
            }

            ApplicationUpdateEvent::~ApplicationUpdateEvent()
            {

            }
    } // namespace Event
    
} // namespace SekaiEngine
