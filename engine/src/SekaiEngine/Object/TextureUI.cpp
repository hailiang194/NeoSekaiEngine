#include "SekaiEngine/Object/TextureUI.h"

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
        }

        void TextureUI::OnEvent(Event::Event &event)
        {
        }

    } // namespace Object

} // namespace SekaiEngine

