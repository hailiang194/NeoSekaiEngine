#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_texture("./icon.png")
    {
    }

    ExampleLayer(const ExampleLayer& layer)
        :m_texture(layer.m_texture)
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
        SekaiEngine::Render::RenderProperties props;
        props.Offset = SekaiEngine::Math::Vector2D(200.0f, 100.0f);
        SekaiEngine::Render::TextureRenderParams params(props, m_texture);
        SekaiEngine::Render::RenderCommand::Render(params);
    }
private:
    SekaiEngine::Render::Texture m_texture;
};


class Texture: public SekaiEngine::Application
{
public:
    Texture()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    Texture(const Texture& Texture)
        :Application(Texture)
    {

    }

    ~Texture()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new Texture();
}