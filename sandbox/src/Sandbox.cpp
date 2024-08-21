#include "SekaiEngine.h"
#include "SekaiEngine/Object/TextureUI.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :textureUI(SekaiEngine::Render::Texture("icon.png"))
    {
    }

    ExampleLayer(const ExampleLayer& layer)
        :textureUI(layer.textureUI)
    {
    }

    ~ExampleLayer()
    {

    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
        textureUI.OnEvent(event);
    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        textureUI.OnUpdate(elipse);
        static SekaiEngine::Math::Vector2D temp;
        temp.X()  += (0.01 * elipse.ToMiliseconds());
        textureUI.SetPosition(temp);
        //textureUI.Self().Offset.X() += (0.01 * elipse.ToMiliseconds());
    }

    void OnRender() override
    {
        textureUI.OnRender();
    }
private:
    SekaiEngine::Object::TextureUI textureUI;
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