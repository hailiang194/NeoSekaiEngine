#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_circle(SekaiEngine::Math::Vector2D(100.0f, 100.0f), 20.0f)
    {
    }

    ExampleLayer(const ExampleLayer& layer)
    {
    }

    ~ExampleLayer()
    {

    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        m_circle.Center.X() = SekaiEngine::Input::GetMouseX();
        m_circle.Center.Y() = SekaiEngine::Input::GetMouseY();
    }

    void OnRender() override
    {
        SekaiEngine::Render::RenderProperties props;
        props.Tint = (SekaiEngine::Render::Color)0xff0000ff;
        SekaiEngine::Render::CircleRenderParams param(props, m_circle);
        SekaiEngine::Render::RenderCommand::Render(param);
    }
private:
    SekaiEngine::Shape::Circle m_circle;
};


class InputMouse: public SekaiEngine::Application
{
public:
    InputMouse()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    InputMouse(const InputMouse& InputMouse)
        :Application(InputMouse)
    {

    }

    ~InputMouse()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new InputMouse();
}