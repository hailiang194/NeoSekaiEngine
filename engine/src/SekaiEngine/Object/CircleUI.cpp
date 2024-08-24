#include "SekaiEngine/Object/CircleUI.h"
#include "SekaiEngine/Render/Params/CircleRenderParams.h"
#include "SekaiEngine/Render/RenderCommand.h"

namespace SekaiEngine
{
    namespace Object
    {
        CircleUI::CircleUI(const Shape::Circle &circle, UI *parent, Render::RenderProperties self)
            :UI(parent, self), m_circle(circle)
        {
            m_self.Scale = Math::Vector2D(circle.Radius, circle.Radius);
            m_self.Offset = m_circle.Center - m_self.Scale;
        }

        CircleUI::CircleUI(const CircleUI &ui)
            :UI(ui), m_circle(ui.m_circle)
        {
        }

        CircleUI &CircleUI::operator=(const CircleUI &ui)
        {
            UI::operator=(ui);
            m_circle = ui.m_circle;
            return (*this);
        }

        CircleUI::~CircleUI()
        {
        }

        void CircleUI::OnUpdate(const Timestep &elipse)
        {
        }

        void CircleUI::OnRender()
        {
            UI::OnRender();
            //SekaiEngine::Render::TextureRenderParams param(m_render, m_texture);
            SekaiEngine::Render::CircleRenderParams param(m_render, m_circle);
            SekaiEngine::Render::RenderCommand::Render(param);
        }

        void CircleUI::OnEvent(Event::Event &event)
        {
        }

        void CircleUI::SetPosition(const Math::Vector2D& position)
        {
            UI::SetPosition(position);
            m_circle.Center = position;
        }

        void CircleUI::SetScale(const Math::Vector2D& scale)
        {
            UI::SetScale(scale);
            m_circle.Radius = scale.X();
            m_self.Offset = m_circle.Center - scale;
        }

        void CircleUI::SetTint(const Render::Color& tint)
        {
            UI::SetTint(tint);
        }

        void CircleUI::SetOrigin(const Math::Vector2D& origin)
        {
            UI::SetOrigin(origin);
        }

        void CircleUI::SetRotation(const float& rotation)
        {
            UI::SetRotation(rotation);
        }
    } // namespace Object
    
} // namespace SekaiEngine
