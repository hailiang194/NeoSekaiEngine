#include "SekaiEngine.h"
#include "SekaiEngine/Object/RectangleUI.h"
#include "SekaiEngine/Input.h"

#include <iostream>

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
        // std::cout << SekaiEngine::Input::GetTotalTouchPoints() << std::endl;
        for(size_t i = 0; i < SekaiEngine::Input::GetTotalTouchPoints(); ++i)
        {
            SekaiEngine::Shape::Circle c(SekaiEngine::Input::GetTouchPointPosition(i), 10.0f);
            
            SekaiEngine::Render::RenderProperties props;
            props.Tint = 0xffffffff;
            SekaiEngine::Render::CircleRenderParams params(props, c);
            SekaiEngine::Render::RenderCommand::Render(params);
        }
    }
    
};


class Sandbox: public SekaiEngine::Application
{
public:
    Sandbox()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    Sandbox(const Sandbox& sandbox)
        :Application(sandbox)
    {

    }

    ~Sandbox()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new Sandbox();
}