#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_circle(SekaiEngine::Math::Vector2D(400.0f, 200.0f), 100.0f),
        m_rect(SekaiEngine::Math::Vector2D(500.0f, 400.0f), 200.0f, 100.0f)
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
    }

    void OnRender() override
    {
        SekaiEngine::Render::RenderProperties circleProps;
        circleProps.Tint = (SekaiEngine::Render::Color)0xff0000ff;
        SekaiEngine::Render::CircleRenderParams circleParam(circleProps, m_circle);
        SekaiEngine::Render::RenderCommand::Render(circleParam);

        SekaiEngine::Render::RenderProperties rectProps;
        rectProps.Tint = (SekaiEngine::Render::Color)0x00ff00ff;
        SekaiEngine::Render::RectangleRenderParams rectParam(rectProps, m_rect);
        SekaiEngine::Render::RenderCommand::Render(rectParam);
    }
private:
    SekaiEngine::Shape::Circle m_circle;
    SekaiEngine::Shape::Rectangle m_rect;
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