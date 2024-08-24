#include "SekaiEngine/Object/GroupUI.h"

namespace SekaiEngine
{
    namespace Object
    {
        GroupUI::GroupUI(UI *parent, Render::RenderProperties self)
            :UI(parent, self)
        {
        }

        GroupUI::GroupUI(const GroupUI &ui)
            :UI(ui)
        {
        }

        GroupUI &GroupUI::operator=(const GroupUI &ui)
        {
            UI::operator=(ui);
            return (*this);
        }

        GroupUI::~GroupUI()
        {
        }

        void GroupUI::OnUpdate(const Timestep &elipse)
        {
            for(std::vector<UI*>::reverse_iterator iter = m_children.rbegin(); iter != m_children.rbegin(); ++iter)
            {
                (*iter)->OnUpdate(elipse);
            }
        }

        void GroupUI::OnRender()
        {
            UI::OnRender();
            for(std::vector<UI*>::reverse_iterator iter = m_children.rbegin(); iter != m_children.rbegin(); ++iter)
            {
                (*iter)->OnRender();
            }
        }

        void GroupUI::OnEvent(Event::Event &event)
        {
            for(std::vector<UI*>::reverse_iterator iter = m_children.rbegin(); iter != m_children.rend(); ++iter)
            {
                (*iter)->OnEvent(event);
            }
        }

        void GroupUI::SetPosition(const Math::Vector2D& position)
        {
            UI::SetPosition(position);
        }

        void GroupUI::SetScale(const Math::Vector2D& scale)
        {
            UI::SetScale(scale);
        }

        void GroupUI::SetTint(const Render::Color& tint)
        {
            UI::SetTint(tint);
        }

        void GroupUI::SetOrigin(const Math::Vector2D& origin)
        {
            UI::SetOrigin(origin);
        }

        void GroupUI::SetRotation(const float& rotation)
        {
            UI::SetRotation(rotation);
        }        
    } // namespace Object
    
} // namespace SekaiEngine
