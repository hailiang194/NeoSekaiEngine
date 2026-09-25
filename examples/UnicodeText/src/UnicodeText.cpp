#include "SekaiEngine.h"

class ExampleLayer : public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
    {
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace("noto-24", "./NotoSansTC-VariableFont_wght.ttf", 24);
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
        SekaiEngine::Application::Instance()->TextEngine().DrawText(
            "Hello World. 你好，新世海的遊戲引擎",
            SekaiEngine::Math::Vector2D(80.0f, 80.0f),
            (SekaiEngine::Render::Color)0xffffffff,
            "noto-24"
        );
    }
private:
};


class UnicodeText : public SekaiEngine::Application
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