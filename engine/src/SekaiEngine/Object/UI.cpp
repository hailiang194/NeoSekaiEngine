#include "SekaiEngine/Object/UI.h"

namespace SekaiEngine
{
    namespace Object
    {
        UI::UI(UI* parent, Render::RenderProperties self)
            :m_parent(parent), m_self(self), m_render(self)
        {

        }

        UI::UI(const UI& ui)
            :m_self(ui.m_self), m_render(ui.m_render), m_parent(nullptr)
        {

        }
        UI& UI::operator=(const UI& ui)
        {
            m_self = ui.m_self;
            m_render = ui.m_render;
            m_parent = nullptr;
            return (*this);
        }

        UI::~UI()
        {

        }

        void UI::OnRender()
        {
            if(m_parent == nullptr)
            {
                m_render = m_self;
                return;
            }
            //@Todo: Compute render properties

        }

        void UI::SetPosition(const Math::Vector2D& position)
        {
            m_self.Offset = position;
        }

        void UI::SetScale(const Math::Vector2D& scale)
        {
            m_self.Scale = scale;
        }
    } // namespace Object
    
} // namespace SekaiEngine
