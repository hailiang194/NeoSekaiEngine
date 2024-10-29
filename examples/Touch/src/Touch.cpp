#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
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
        for(size_t i = 0; i < SekaiEngine::Input::GetTotalTouchPoints(); ++i)
        {
            SekaiEngine::Shape::Circle c(SekaiEngine::Input::GetTouchPointPosition(i), 10.0f);

            SekaiEngine::Render::RenderProperties props;
            props.Tint = 0xffffffff;
            SekaiEngine::Render::CircleRenderParams params(props, c);
            SekaiEngine::Render::RenderCommand::Render(params);
        }
    }
private:
};


class Touch: public SekaiEngine::Application
{
public:
    Touch()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    Touch(const Touch& Touch)
        :Application(Touch)
    {

    }

    ~Touch()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new Touch();
}