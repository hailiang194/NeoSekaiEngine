#include "SekaiEngine.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :Layer("Hello friend"), camera()
    {

    }

    ExampleLayer(const ExampleLayer& layer)
        :Layer(layer), camera(layer.camera)
    {

    }

    ~ExampleLayer()
    {

    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        SekaiEngine::Render::RenderCommand::SetClearColor((SekaiEngine::Render::Color)0xffffffff);
        SekaiEngine::Render::RenderCommand::Clear();

        SekaiEngine::Render::Renderer::BeginScreen(camera);

        SekaiEngine::Render::Renderer::Render();

        SekaiEngine::Render::Renderer::EndScreen();
    }
private:
    SekaiEngine::Render::Camera2D camera;
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