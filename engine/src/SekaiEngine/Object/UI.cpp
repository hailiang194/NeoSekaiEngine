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
            Render::UpdateRenderProperies(m_parent->m_render, m_self, m_render);

        }

        void UI::SetPosition(const Math::Vector2D& position)
        {
            m_self.Offset = position;
        }

        void UI::SetScale(const Math::Vector2D& scale)
        {
            m_self.Scale = scale;
        }

        void UI::SetTint(const Render::Color& tint)
        {
            m_self.Tint = tint;
        }

        void UI::SetOrigin(const Math::Vector2D& origin)
        {
            m_self.Origin = origin;
        }

        void UI::SetRotation(const float& rotation)
        {
            m_self.Rotation = rotation;
        }
    } // namespace Object
    
} // namespace SekaiEngine
