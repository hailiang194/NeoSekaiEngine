#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_sound("./Mission Completed.wav"),
        m_music("./Stream Loops 2023-11-29.ogg")
    {
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
        std::chrono::time_point<std::chrono::high_resolution_clock> current = std::chrono::high_resolution_clock::now();
        static std::chrono::time_point<std::chrono::high_resolution_clock> lastPClicked = current;

        if(m_sound.IsValid() && SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_SPACE))
        {
            m_sound.Play();
        }

        if(m_music.Played().ToMiliseconds() >= m_music.Length().ToMiliseconds())
        {
            m_music.Seek((SekaiEngine::Timestep)0.0f);
        }

        if(m_music.IsValid() && SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KeyboardKey::KEY_P)) 
        {
            if((std::chrono::duration_cast<std::chrono::milliseconds>(current - lastPClicked).count()) >= 1000)
            {
                if(m_music.IsPlaying())
                {
                    m_music.Pause();
                }
                else
                {
                    m_music.Play();
                }

                lastPClicked = current;
            }
        }
    }

    void OnRender() override
    {
        SekaiEngine::Render::API::DrawText("Press Space to play sound", 
            SekaiEngine::Math::Vector2D(200.0f, 180.0f), 
            (SekaiEngine::Render::Color)0xff0000ff,
            20
        );

        SekaiEngine::Render::API::DrawText("Press P to play/pause music", 
            SekaiEngine::Math::Vector2D(200.0f, 210.0f), 
            (SekaiEngine::Render::Color)0xff0000ff,
            20
        );


        SekaiEngine::Shape::Rectangle totalLength(SekaiEngine::Math::Vector2D(200, 500), 700, 20);
        SekaiEngine::Render::RenderProperties totalLengthProps;
        totalLengthProps.Tint = 0x00ffffff;
        SekaiEngine::Render::RectangleRenderParams totalLengthParams(totalLengthProps, totalLength);
        SekaiEngine::Render::RenderCommand::Render(totalLengthParams);

        SekaiEngine::Shape::Rectangle playingLength(SekaiEngine::Math::Vector2D(200, 500), 700 * m_music.Played().ToMiliseconds() / m_music.Length().ToMiliseconds(), 20);
        SekaiEngine::Render::RenderProperties playingLengthProps;
        playingLengthProps.Tint = 0xff00ffff;
        SekaiEngine::Render::RectangleRenderParams playingLengthParams(playingLengthProps, playingLength);
        SekaiEngine::Render::RenderCommand::Render(playingLengthParams);
        
    }
private:
    SekaiEngine::Sound::Sound m_sound;
    SekaiEngine::Sound::MusicStream m_music;

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