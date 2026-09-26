#include "SekaiEngine.h"

/**
 * @brief The script the labels are shown in, switched every LANGUAGE_SECONDS
 *
 */
enum class Language
{
    English,
    Vietnamese,
    TraditionalChinese
};

static const int LANGUAGE_COUNT = 3;
static const float LANGUAGE_SECONDS = 5.0f;

/**
 * @brief One label per draw, in each of the three scripts
 *
 * @note Every entry is a string literal, so the pointer a recorded DrawCmd holds
 * has static storage duration and outlives the frame's command buffer.
 */
static const char* const LABELS[4][LANGUAGE_COUNT] =
{
    { "Circle", "Hình tròn", "圓形" },
    { "Rectangle", "Hình chữ nhật", "長方形" },
    { "Whole texture", "Toàn bộ kết cấu", "完整紋理" },
    { "Atlas sub-rectangle", "Vùng của bản đồ", "圖集子區域" }
};

class ExampleLayer : public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :m_circle(SekaiEngine::Math::Vector2D(250.0f, 180.0f), 70.0f),
        m_rect(SekaiEngine::Math::Vector2D(600.0f, 120.0f), 220.0f, 130.0f),
        m_texture("./icon.png"),
        m_language(0),
        m_languageTimer(0.0f)
    {
        //Loaded once at startup and never resized, so the glyph cache stays keyed
        //consistently by face name and size for the life of the example.
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace(
            "noto-24", "./NotoSansTC-VariableFont_wght.ttf", 24
        );
    }

    ExampleLayer(const ExampleLayer& layer)
        :m_circle(layer.m_circle),
        m_rect(layer.m_rect),
        m_texture(layer.m_texture)
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
        m_languageTimer += elipse.ToSeconds();
        if(m_languageTimer >= LANGUAGE_SECONDS)
        {
            m_languageTimer -= LANGUAGE_SECONDS;
            m_language = (m_language + 1) % LANGUAGE_COUNT;
        }
    }

    void OnRender() override
    {
        using namespace SekaiEngine;
        using namespace SekaiEngine::Math;
        using namespace SekaiEngine::Render;

        //Recording is plain appending: every draw below lands in the same buffer and is
        //replayed in this exact order at the end of the frame, so the order below is the
        //composite order. The line of mixed Latin and Traditional Chinese goes in first
        //and the rectangle is recorded over its middle, so the rectangle wins; the four
        //labels go in after the draw they belong to, and the first one lands inside the
        //circle, so it wins over the circle.

        //One run of text mixing Latin and Traditional Chinese, so the face has to fall
        //back across scripts mid-string and carry the advance widths correctly.
        RenderCommand::Record(MakeLabel("DrawCmds — 繪圖指令", Vector2D(500.0f, 260.0f)));

        RenderProperties circleProps;
        circleProps.Tint = (Color)0xff0000ff;
        RenderCommand::Record(MakeCircleCmd(circleProps, m_circle, 30.0f, 330.0f, 48));
        RenderCommand::Record(MakeLabel(LABELS[0][m_language], Vector2D(200.0f, 190.0f)));

        //The origin is the rect's half size, because the renderer rotates a rect about
        //its own position plus the origin, so this spins the rect about its centre
        //instead of swinging it off to one side.
        RenderProperties rectProps;
        rectProps.Tint = (Color)0x00ff00ff;
        rectProps.Origin = Vector2D(110.0f, 65.0f);
        rectProps.Rotation = 20.0f;
        RenderCommand::Record(MakeRectCmd(rectProps, m_rect));
        RenderCommand::Record(MakeLabel(LABELS[1][m_language], Vector2D(640.0f, 300.0f)));

        RenderProperties wholeTextureProps;
        wholeTextureProps.Offset = Vector2D(60.0f, 320.0f);
        RenderCommand::Record(MakeTexCmd(wholeTextureProps, m_texture));
        RenderCommand::Record(MakeLabel(LABELS[2][m_language], Vector2D(60.0f, 540.0f)));

        //An explicit source region drawn at a destination size of its own: only the
        //middle quarter of the icon appears, and the quarter scale shrinks that quarter
        //to a quarter of the width of the whole-texture draw above it.
        RenderProperties atlasProps;
        atlasProps.Offset = Vector2D(300.0f, 320.0f);
        atlasProps.Scale = Vector2D(0.25f, 0.25f);
        RenderCommand::Record(MakeTexCmd(atlasProps, m_texture,
            Vector2D(), Shape::Rectangle(Vector2D(48.0f, 48.0f), 96.0f, 96.0f)
        ));
        RenderCommand::Record(MakeLabel(LABELS[3][m_language], Vector2D(300.0f, 400.0f)));
    }
private:
    /**
     * @brief Make the command for a white label, the draw this example is about
     *
     */
    static SekaiEngine::Render::DrawCmd MakeLabel(const char* text, const SekaiEngine::Math::Vector2D& position)
    {
        SekaiEngine::Render::RenderProperties props;
        props.Tint = (SekaiEngine::Render::Color)0xffffffff;
        return SekaiEngine::Render::MakeTextCmd(props, text, position, "noto-24");
    }

    SekaiEngine::Shape::Circle m_circle;
    SekaiEngine::Shape::Rectangle m_rect;
    SekaiEngine::Render::Texture m_texture;
    int m_language;
    float m_languageTimer;
};


class DrawCmd : public SekaiEngine::Application
{
public:
    DrawCmd()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    DrawCmd(const DrawCmd& DrawCmd)
        :Application(DrawCmd)
    {

    }

    ~DrawCmd()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new DrawCmd();
}
