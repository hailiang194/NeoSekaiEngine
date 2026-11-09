#include "SekaiEngine.h"

#include <cstdint>
#include <cstdio>
#include <random>

namespace
{
    const int RECT_COUNT = 120;
    const float WORLD_HALF = 2000.0f;
    const float CAMERA_SPEED = 400.0f;
    const uint32_t RANDOM_SEED = 20241109u;
    const char* const FONT_NAME = "noto-24";
    const char* const FONT_PATH = "./NotoSansTC-VariableFont_wght.ttf";
} // namespace

class ExampleLayer : public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_camera(), m_frame(0)
    {
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace(
            FONT_NAME, FONT_PATH, 24
        );

        const SekaiEngine::IWindow& window = SekaiEngine::Application::Instance()->Window();
        m_camera.Offset = SekaiEngine::Math::Vector2D(
            window.GetWidth() * 0.5f, window.GetHeight() * 0.5f
        );

        std::mt19937 rng(RANDOM_SEED);
        std::uniform_real_distribution<float> position(-WORLD_HALF, WORLD_HALF);
        std::uniform_real_distribution<float> size(20.0f, 160.0f);
        std::uniform_int_distribution<int> channel(0, 255);

        for(int i = 0; i < RECT_COUNT; ++i)
        {
            m_world[i].rect = SekaiEngine::Shape::Rectangle(
                SekaiEngine::Math::Vector2D(position(rng), position(rng)),
                size(rng), size(rng)
            );

            uint32_t code = (static_cast<uint32_t>(channel(rng)) << 24)
                | (static_cast<uint32_t>(channel(rng)) << 16)
                | (static_cast<uint32_t>(channel(rng)) << 8)
                | 0xffu;
            m_world[i].color = SekaiEngine::Render::Color(code);
        }
    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        using namespace SekaiEngine::Input;
        using SekaiEngine::Math::Vector2D;

        Vector2D direction;
        if(IsKeyPressed(KEY_A))
            direction = direction + Vector2D(-1.0f, 0.0f);
        if(IsKeyPressed(KEY_D))
            direction = direction + Vector2D(1.0f, 0.0f);
        if(IsKeyPressed(KEY_W))
            direction = direction + Vector2D(0.0f, -1.0f);
        if(IsKeyPressed(KEY_S))
            direction = direction + Vector2D(0.0f, 1.0f);

        m_camera.Position = m_camera.Position
            + direction * (CAMERA_SPEED * elipse.ToSeconds());
    }

    void OnRender() override
    {
        using namespace SekaiEngine;
        using namespace SekaiEngine::Math;
        using namespace SekaiEngine::Render;

        RenderCommand::Record(MakeBeginCameraCmd(m_camera));

        for(int i = 0; i < RECT_COUNT; ++i)
        {
            RenderProperties props;
            props.Tint = m_world[i].color;
            RenderCommand::Record(MakeRectCmd(props, m_world[i].rect));
        }

        RenderCommand::Record(MakeEndCameraCmd());

        int slot = m_frame;
        snprintf(m_labels[slot], sizeof(m_labels[slot]), "Camera: (%.1f, %.1f)",
            m_camera.Position.X(), m_camera.Position.Y()
        );

        RenderProperties labelProps;
        labelProps.Tint = (Color)0xffffffff;
        RenderCommand::Record(MakeTextCmd(
            labelProps, m_labels[slot], Vector2D(10.0f, 20.0f), FONT_NAME
        ));

        m_frame = 1 - m_frame;
    }

private:
    struct WorldRect
    {
        SekaiEngine::Shape::Rectangle rect;
        SekaiEngine::Render::Color color;
    };

    SekaiEngine::Render::Camera2D m_camera;
    WorldRect m_world[RECT_COUNT];
    char m_labels[2][64];
    int m_frame;
};

class Camera : public SekaiEngine::Application
{
public:
    Camera()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new Camera();
}
