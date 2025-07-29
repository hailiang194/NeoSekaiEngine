#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
    {
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace("arial-16", "./Arial.ttf", 16);
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


class UnicodeText: public SekaiEngine::Application
{
public:
    UnicodeText()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    UnicodeText(const UnicodeText& UnicodeText)
        :Application(UnicodeText)
    {

    }

    ~UnicodeText()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new UnicodeText();
}