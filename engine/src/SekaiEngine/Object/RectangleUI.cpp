#include "SekaiEngine/Object/RectangleUI.h"
#include "SekaiEngine/Render/RenderCommand.h"

namespace SekaiEngine
{
    namespace Object
    {
        RectangleUI::RectangleUI(const Shape::Rectangle &rectangle, UI *parent, Render::RenderProperties self)
            :UI(parent, self), m_rectangle(rectangle)
        {
        }

        RectangleUI::RectangleUI(const RectangleUI &ui)
            :UI(ui), m_rectangle(ui.m_rectangle)
        {
        }

        RectangleUI &RectangleUI::operator=(const RectangleUI &ui)
        {
            UI::operator=(ui);
            m_rectangle = ui.m_rectangle;
            return (*this);
        }

        RectangleUI::~RectangleUI()
        {
        }

        void RectangleUI::OnUpdate(const Timestep &elipse)
        {
        }

        void RectangleUI::OnRender()
        {
            UI::OnRender();
            //SekaiEngine::Render::TextureRenderParams param(m_render, m_texture);
            SekaiEngine::Render::RectangleRenderParams param(m_render, m_rectangle);
            SekaiEngine::Render::RenderCommand::Render(param);
        }

        void RectangleUI::OnEvent(Event::Event &event)
        {
        }

        void RectangleUI::SetPosition(const Math::Vector2D& position)
        {
            UI::SetPosition(position);
        }

        void RectangleUI::SetScale(const Math::Vector2D& scale)
        {
            UI::SetScale(scale);
        }

        void RectangleUI::SetTint(const Render::Color& tint)
        {
            UI::SetTint(tint);
        }

        void RectangleUI::SetOrigin(const Math::Vector2D& origin)
        {
            UI::SetOrigin(origin);
        }

        void RectangleUI::SetRotation(const float& rotation)
        {
            UI::SetRotation(rotation);
        }
    } // namespace Object
    
} // namespace SekaiEngine
