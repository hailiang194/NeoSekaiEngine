#include "SekaiEngine.h"

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_sound("./Mission Completed.wav"),
        m_music("./Stream Loops 2023-11-29.ogg")
    {
        //Recorded text replays through the text engine, so it needs a face loaded
        //before the loop starts - the update thread may not load one while a frame
        //is being replayed.
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace(
            "noto-20", "./NotoSansTC-VariableFont_wght.ttf", 20
        );
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
        //The labels are recorded rather than drawn straight to the renderer:
        //OnRender runs on the update thread, which does not hold the graphics
        //context, and a recorded draw is the only thing that may cross to the
        //thread that replays the frame.
        SekaiEngine::Render::RenderProperties labelProps;
        labelProps.Tint = (SekaiEngine::Render::Color)0xff0000ff;

        SekaiEngine::Render::RenderCommand::Record(SekaiEngine::Render::MakeTextCmd(
            labelProps, "Press Space to play sound", SekaiEngine::Math::Vector2D(200.0f, 180.0f), "noto-20"
        ));
        SekaiEngine::Render::RenderCommand::Record(SekaiEngine::Render::MakeTextCmd(
            labelProps, "Press P to play/pause music", SekaiEngine::Math::Vector2D(200.0f, 210.0f), "noto-20"
        ));

        SekaiEngine::Shape::Rectangle totalLength(SekaiEngine::Math::Vector2D(200, 500), 700, 20);
        SekaiEngine::Render::RenderProperties totalLengthProps;
        totalLengthProps.Tint = 0x00ffffff;
        SekaiEngine::Render::RenderCommand::Record(SekaiEngine::Render::MakeRectCmd(totalLengthProps, totalLength));

        SekaiEngine::Shape::Rectangle playingLength(SekaiEngine::Math::Vector2D(200, 500), 700 * m_music.Played().ToMiliseconds() / m_music.Length().ToMiliseconds(), 20);
        SekaiEngine::Render::RenderProperties playingLengthProps;
        playingLengthProps.Tint = 0xff00ffff;
        SekaiEngine::Render::RenderCommand::Record(SekaiEngine::Render::MakeRectCmd(playingLengthProps, playingLength));
        
    }
private:
    SekaiEngine::Sound::Sound m_sound;
    SekaiEngine::Sound::MusicStream m_music;

};


class SoundMusic: public SekaiEngine::Application
{
public:
    SoundMusic()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    SoundMusic(const SoundMusic& SoundMusic)
        :Application(SoundMusic)
    {

    }

    ~SoundMusic()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new SoundMusic();
}
