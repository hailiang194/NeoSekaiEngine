#include "SekaiEngine.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :Layer("Hello friend"), camera(), rect(SekaiEngine::Math::Vector2D(), 100, 50)
    {

    }

    ExampleLayer(const ExampleLayer& layer)
        :Layer(layer), camera(layer.camera), rect(layer.rect)
    {

    }

    ~ExampleLayer()
    {

    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_A))
            rect.Position.X() -= (100 * elipse.ToSeconds());
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_D))
            rect.Position.X() += (100 * elipse.ToSeconds());

        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_W))
            rect.Position.Y() -= (100 * elipse.ToSeconds());
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_S))
            rect.Position.Y() += (100 * elipse.ToSeconds());

        SekaiEngine::Render::RenderCommand::SetClearColor((SekaiEngine::Render::Color)0xffffffff);
        SekaiEngine::Render::RenderCommand::Clear();

        SekaiEngine::Render::Renderer::BeginScreen(camera);

        SekaiEngine::Render::RenderProperties props;
        props.Tint = 0xff0000ff;
        SekaiEngine::Render::RectangleRenderParams params(props, rect);
        SekaiEngine::Render::API::DrawRect(params.DrawRect(), params.Tint(), params.Origin(), params.Rotation());

        SekaiEngine::Render::Renderer::EndScreen();
    }
private:
    SekaiEngine::Render::Camera2D camera;
    SekaiEngine::Shape::Rectangle rect;
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