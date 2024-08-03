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
            //@Todo: Compute render properties

        }
    } // namespace Object
    
} // namespace SekaiEngine
