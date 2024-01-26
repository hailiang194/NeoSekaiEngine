#include "SekaiEngine.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :Layer("Hello friend"), camera(), texture("icon.png"), position()
    {

    }

    ExampleLayer(const ExampleLayer& layer)
        :Layer(layer), camera(layer.camera), texture(layer.texture), position(layer.position)
    {

    }

    ~ExampleLayer()
    {

    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_A))
            position.X() -= (100 * elipse.ToSeconds());
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_D))
            position.X() += (100 * elipse.ToSeconds());

        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_W))
            position.Y() -= (100 * elipse.ToSeconds());
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_S))
            position.Y() += (100 * elipse.ToSeconds());

        SekaiEngine::Render::RenderCommand::SetClearColor((SekaiEngine::Render::Color)0xffffffff);
        SekaiEngine::Render::RenderCommand::Clear();

        SekaiEngine::Render::Renderer::BeginScreen(camera);

        SekaiEngine::Render::RenderProperties props;
        SekaiEngine::Render::TextureRenderParams params(props, texture, position);
        SekaiEngine::Render::API::DrawTexture(params.Texture(), params.Tint(), params.Source(), params.Destination(), params.Origin(), params.Rotation());

        SekaiEngine::Render::Renderer::EndScreen();
    }
private:
    SekaiEngine::Render::Camera2D camera;
    SekaiEngine::Render::Texture texture;
    SekaiEngine::Math::Vector2D position;
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