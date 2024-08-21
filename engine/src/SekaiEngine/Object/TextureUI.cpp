#include "SekaiEngine/Object/TextureUI.h"
#include "SekaiEngine/Render/Params/TextureRenderParams.h"
#include "SekaiEngine/Render/RenderCommand.h"

namespace SekaiEngine
{
    namespace Object
    {
        TextureUI::TextureUI(const Render::Texture &texture, UI *parent, Render::RenderProperties self)
            :UI(parent, self), m_texture(texture)
        {
        }

        TextureUI::TextureUI(const TextureUI &ui)
            :UI(ui), m_texture(ui.m_texture)
        {
        }

        TextureUI &TextureUI::operator=(const TextureUI &ui)
        {
            UI::operator=(ui);
            m_texture = ui.m_texture;
            return (*this);
        }

        TextureUI::~TextureUI()
        {
        }

        void TextureUI::OnUpdate(const Timestep &elipse)
        {
        }

        void TextureUI::OnRender()
        {
            UI::OnRender();
            SekaiEngine::Render::TextureRenderParams param(m_render, m_texture);
            SekaiEngine::Render::RenderCommand::Render(param);
        }

        void TextureUI::OnEvent(Event::Event &event)
        {
        }

        void TextureUI::SetPosition(const Math::Vector2D& position)
        {
            UI::SetPosition(position);
        }

        void TextureUI::SetScale(const Math::Vector2D& scale)
        {
            UI::SetScale(scale);
        }

    } // namespace Object

} // namespace SekaiEngine

