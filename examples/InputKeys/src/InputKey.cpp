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
        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_A))
        {
            m_circle.Center.X() -= 2.0f;
        }
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_D))
        {
            m_circle.Center.X() += 2.0f;
        }

        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_W))
        {
            m_circle.Center.Y() -= 2.0f;
        }
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_S))
        {
            m_circle.Center.Y() += 2.0f;
        }
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


class InputKey: public SekaiEngine::Application
{
public:
    InputKey()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    InputKey(const InputKey& InputKey)
        :Application(InputKey)
    {

    }

    ~InputKey()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new InputKey();
}