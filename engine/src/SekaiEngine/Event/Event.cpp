#include "SekaiEngine/Event/Event.h"

namespace SekaiEngine
{
    namespace Event
    {
        Event::Event()
            :Handled(false)
        {

        }

        Event::Event(const Event& event)
            :Handled(event.Handled)
        {

        }

        Event& Event::operator=(const Event& event)
        {
            Handled = event.Handled;
            return (*this);
        }

        Event::~Event()
        {

        }
    } // namespace Event
    
} // namespace SekaiEngine
