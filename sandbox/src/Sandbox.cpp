#include "SekaiEngine.h"
#include "SekaiEngine/Object/CircleUI.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :circleUI(SekaiEngine::Shape::Circle(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 10.0f))
    {
    }

    ExampleLayer(const ExampleLayer& layer)
        :circleUI(layer.circleUI)
    {
    }

    ~ExampleLayer()
    {

    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
        circleUI.OnEvent(event);
    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        circleUI.OnUpdate(elipse);
        static SekaiEngine::Math::Vector2D temp;
        temp.X()  += (0.01 * elipse.ToMiliseconds());
        circleUI.SetScale(temp);
        //textureUI.Self().Offset.X() += (0.01 * elipse.ToMiliseconds());
    }

    void OnRender() override
    {
        circleUI.OnRender();
    }
private:
    SekaiEngine::Object::CircleUI circleUI;
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