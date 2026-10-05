#include <cstdio>
#include <variant>

#include "SekaiEngine.h"
#include "SekaiEngine/Animation/Transitions/Back.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"
#include "SekaiEngine/Animation/Transitions/Elastic.h"
#include "SekaiEngine/Animation/Transitions/Bounce.h"
#include "SekaiEngine/Animation/Transitions/Expo.h"
#include "SekaiEngine/Animation/Transitions/Cubic.h"
#include "SekaiEngine/Animation/Transitions/Quad.h"
#include "SekaiEngine/Animation/Transitions/Sine.h"
#include "SekaiEngine/Animation/Transitions/Types.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Shape/Circle.h"

/**
 * @brief How the example's one animated value moves
 *
 */
/*!< The radius the circle starts at, and the one it ends at*/
static const float START_RADIUS = 30.0f;
static const float END_RADIUS = 70.0f;
/*!< The seconds the transition takes to cover the whole distance, 6 in this case*/
static const float DURATION_SECONDS = 6.0f;
/*!< Where the name of the curve being played is drawn, just inside the top-left*/
static const float LABEL_X = 10.0f;
static const float LABEL_Y = 30.0f;

/**
 * @brief One entry of the cycle: the family to play and the name to put on screen while
 * it plays
 *
 * @note Which family is a tag rather than a stored object, because the curve itself lives
 * in the variant below. Storing one of each family here would mean a second copy of every
 * curve that nothing ever reads. The direction needs no entry of its own for the same
 * reason: it is part of the curve's type, which the tag already picks.
 */
struct CurveEntry
{
    const char* name;
    /*!< Which alternative of the variant below this entry means, the alias that already
      carries the direction this entry plays.*/
    enum Family {QUAD, CUBIC, SINE, EXPO, BACK, ELASTIC, BOUNCE} family;
};

/**
 * @brief The curves the example cycles through, chosen so both the plain shapes and the two
 * overshooting families are visible on one screen
 *
 */
static const CurveEntry CURVES[] =
{
    { "Quad In", CurveEntry::QUAD },
    { "Cubic InOut", CurveEntry::CUBIC },
    { "Sine Out", CurveEntry::SINE },
    { "Expo In", CurveEntry::EXPO },
    { "Back InOut", CurveEntry::BACK },
    { "Elastic Out", CurveEntry::ELASTIC },
    { "Bounce In", CurveEntry::BOUNCE }
};
static const int CURVE_COUNT = 7;

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    /**
     * @brief The circle starts at its start radius, and the first curve of the cycle drives
     * it to its end radius
     *
     * @note The circle is declared before the handler on purpose: the handler writes to the
     * circle, and the members are built in the order they are declared, so the circle is
     * already there when the handler is created.
     */
    ExampleLayer()
      :m_circle(SekaiEngine::Math::Vector2D(200.0f, 200.0f), START_RADIUS),
      m_handler([this](const float& radius){ m_circle.Radius = radius; }),
      m_transition(SekaiEngine::Animation::Quad<SekaiEngine::Animation::Ease::In>(START_RADIUS, END_RADIUS, SekaiEngine::Timestep(DURATION_SECONDS),
        m_handler)),
      m_index(0),
      m_label()
    {
        //Loaded once at startup and never resized, so the glyph cache stays keyed
        //consistently by face name and size for the life of the example.
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace(
            "noto-24", "./NotoSansTC-VariableFont_wght.ttf", 24
        );
        std::visit([](auto& curve)
        { 
            curve.Start(); 

        }, m_transition);

        snprintf(m_label, sizeof(m_label), "%s", CURVES[m_index].name);
    }

    ExampleLayer(const ExampleLayer& layer)
      :m_circle(layer.m_circle),
      m_handler(layer.m_handler),
      m_transition(layer.m_transition),
      m_index(layer.m_index),
      m_label()
    {
    }

    ~ExampleLayer()
    {

    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
    }

    /**
     * @brief Hand the frame's elapsed time to the current transition, and move on to the
     * next curve in the cycle once it has finished
     *
     * @note A generic lambda handed to std::visit, rather than a call on the variant:
     * whichever family is currently held is a Transition, so all of them answer to Update,
     * and the visitor does not care which one it has been given.
     */
    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        std::visit([&elipse](auto& curve){ curve.Update(elipse); }, m_transition);

        if(!IsFinished())
            return;

        m_index = (m_index + 1) % CURVE_COUNT;
        snprintf(m_label, sizeof(m_label), "%s", CURVES[m_index].name);
        PlayCurrent();
    }

    void OnRender() override
    {
        /*Recorded, not drawn: the command goes into the frame's buffer and is replayed at
          the end of the frame, so every draw below lands in one pass in this same order.*/
        SekaiEngine::Render::RenderProperties props;
        props.Tint = (SekaiEngine::Render::Color)0xff0000ff;
        SekaiEngine::Render::RenderCommand::Record(
            SekaiEngine::Render::MakeCircleCmd(props, m_circle));

        // snprintf(m_label, sizeof(m_label), "%s", CURVES[m_index].name);

        SekaiEngine::Render::RenderProperties labelProps;
        labelProps.Tint = (SekaiEngine::Render::Color)0xffffffff;
        SekaiEngine::Render::RenderCommand::Record(
            SekaiEngine::Render::MakeTextCmd(labelProps, m_label,
              SekaiEngine::Math::Vector2D(LABEL_X, LABEL_Y), "noto-24"));
    }
private:
    /**
     * @brief Build the transition the current entry names, over the same distance, and
     * start it
     *
     */
    void PlayCurrent()
    {
        const CurveEntry& entry = CURVES[m_index];
        const SekaiEngine::Timestep duration(DURATION_SECONDS);

        /*A switch over the tag, because each family is its own type and emplace is the one
          call that fills whichever alternative is named. Every case builds the same
          transition and hands it the same handler, so only the family differs. Each
          alternative is the alias above, which already names the direction its entry plays,
          so no direction is passed at construction.*/
        switch(entry.family)
        {
            case CurveEntry::QUAD:
                m_transition.emplace<SekaiEngine::Animation::Quad<SekaiEngine::Animation::Ease::In>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
            case CurveEntry::CUBIC:
                m_transition.emplace<SekaiEngine::Animation::Cubic<SekaiEngine::Animation::Ease::InOut>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
            case CurveEntry::SINE:
                m_transition.emplace<SekaiEngine::Animation::Sine<SekaiEngine::Animation::Ease::Out>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
            case CurveEntry::EXPO:
                m_transition.emplace<SekaiEngine::Animation::Expo<SekaiEngine::Animation::Ease::In>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
            case CurveEntry::BACK:
                m_transition.emplace<SekaiEngine::Animation::Back<SekaiEngine::Animation::Ease::InOut>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
            case CurveEntry::ELASTIC:
                m_transition.emplace<SekaiEngine::Animation::Elastic<SekaiEngine::Animation::Ease::Out>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
            case CurveEntry::BOUNCE:
            default:
                m_transition.emplace<SekaiEngine::Animation::Bounce<SekaiEngine::Animation::Ease::In>>(START_RADIUS, END_RADIUS, duration, m_handler);
                break;
        }

        std::visit([](auto& curve){ curve.Start(); }, m_transition);
    }

    /**
     * @brief Whether the current curve has run out of time
     *
     */
    bool IsFinished() const
    {
        return std::visit([](const auto& curve){ return curve.IsFinish(); }, m_transition);
    }

    SekaiEngine::Shape::Circle m_circle;
    /*!< The handler the current curve reports to, built once and reused by every curve the
      cycle plays, so each new curve is not handed a new one.*/
    SekaiEngine::Animation::OnUpdateHandler m_handler;
    SekaiEngine::Animation::TransitionVariant m_transition;
    int m_index;
    /*!< The name of the curve being played. A member, not a local in OnRender, because a
      recorded command keeps the pointer it was handed and replays it after that function
      has returned.*/
    char m_label[32];
};

class Animation: public SekaiEngine::Application
{
public:
    Animation()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    Animation(const Animation& animation)
        :Application(animation)
    {

    }

    ~Animation()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new ::Animation();
}
