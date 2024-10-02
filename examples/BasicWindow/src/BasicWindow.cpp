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
    }
private:
};


class BasicWindow: public SekaiEngine::Application
{
public:
    BasicWindow()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    BasicWindow(const BasicWindow& BasicWindow)
        :Application(BasicWindow)
    {

    }

    ~BasicWindow()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new BasicWindow();
}