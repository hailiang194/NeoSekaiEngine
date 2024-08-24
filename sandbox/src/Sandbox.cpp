#include "SekaiEngine.h"
#include "SekaiEngine/Object/RectangleUI.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :rectangleUI(SekaiEngine::Shape::Rectangle(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 10.0f, 20.0f))
    {
    }

    ExampleLayer(const ExampleLayer& layer)
        :rectangleUI(layer.rectangleUI)
    {
    }

    ~ExampleLayer()
    {

    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
        rectangleUI.OnEvent(event);
    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        rectangleUI.OnUpdate(elipse);
        static SekaiEngine::Math::Vector2D temp(10.0f, 10.0f);
        //temp.X()  += (10.0f * elipse.ToMiliseconds());
        rectangleUI.SetScale(temp);
        //textureUI.Self().Offset.X() += (0.01 * elipse.ToMiliseconds());
    }

    void OnRender() override
    {
        rectangleUI.OnRender();
    }
private:
    SekaiEngine::Object::RectangleUI rectangleUI;
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