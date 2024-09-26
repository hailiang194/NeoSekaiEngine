#include "SekaiEngine.h"
#include "SekaiEngine/Object/RectangleUI.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_sound("./Mission Completed.wav"),
        m_music("./Stream Loops 2023-11-29.ogg")
    {
        m_music.Play();
    }

    ExampleLayer(const ExampleLayer& layer)
        :m_sound(layer.m_sound), m_music(layer.m_music)
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
        if(m_sound.IsValid() && SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_SPACE))
        {
            m_sound.Play();
        }
    }

    void OnRender() override
    {
        SekaiEngine::Render::API::DrawText("Press Space to play sound", 
            SekaiEngine::Math::Vector2D(200.0f, 180.0f), 
            (SekaiEngine::Render::Color)0xff0000ff,
            20
        );
    }
private:
    SekaiEngine::Sound::Sound m_sound;
    SekaiEngine::Sound::MusicStream m_music;
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