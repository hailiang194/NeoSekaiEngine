#include <cstddef>
#include <cstdio>
#include <map>
#include <memory>
#include <vector>

#include "SekaiEngine.h"
#include "SekaiEngine/Animation/Transitions/Back.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"
#include "SekaiEngine/Animation/Transitions/Elastic.h"
#include "SekaiEngine/Animation/Transitions/Bounce.h"
#include "SekaiEngine/Animation/Transitions/Expo.h"
#include "SekaiEngine/Animation/Transitions/Cubic.h"
#include "SekaiEngine/Animation/Transitions/Quad.h"
#include "SekaiEngine/Animation/Transitions/Sine.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Shape/Circle.h"

/**
 * @brief How the example's one animated value moves
 *
 */
/*!< The radius the circle starts at, and the one it ends at*/
static const float START_RADIUS = 30.0f;
static const float END_RADIUS = 70.0f;
/*!< The seconds each curve takes to cover the whole distance, 6 in this case*/
static const float DURATION_SECONDS = 6.0f;
/*!< Where the name of the curve being played is drawn, just inside the top-left*/
static const float LABEL_X = 10.0f;
static const float LABEL_Y = 30.0f;

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    /**
     * @brief Build the seven curves as one sequence inside a repeat of forever, hand it to
     * the engine's animation manager, and tell the manager what to do when a curve ends
     *
     * @note The circle is declared before anything that writes to it, and the members are
     * built in the order they are declared, so the circle is already there when the first
     * handler is created.
     *
     * @note The endless cycle comes from the `Repeat` of forever, not from the end handler:
     * the repeat restarts its sequence on its own, and the manager reports each curve of it
     * as that curve ends.
     */
    ExampleLayer()
      :m_circle(SekaiEngine::Math::Vector2D(200.0f, 200.0f), START_RADIUS),
      m_label(), m_curveNames()
    {
        //Loaded once at startup and never resized, so the glyph cache stays keyed
        //consistently by face name and size for the life of the example.
        SekaiEngine::Application::Instance()->TextEngine().LoadFontFace(
            "noto-24", "./NotoSansTC-VariableFont_wght.ttf", 24
        );

        SekaiEngine::Application::Instance()->Animator().OnTransitionEnd(
          [this](const SekaiEngine::Animation::Node& curve)
          {
              /*The manager reports every curve as it ends, so the label is refreshed here,
                once per boundary rather than on every frame. It names the curve that is now
                running — the one after the curve that just ended, wrapping back to the
                first at the end of the cycle — so the label is on the current curve and not
                the one before it. The names are held against the curve objects themselves,
                because a handler is handed the curve and not its name.*/
              for(std::size_t played = 0; played < m_curveOrder.size(); played++)
              {
                  if(m_curveOrder[played] != &curve)
                      continue;

                  const std::size_t current = (played + 1) % m_curveOrder.size();
                  snprintf(m_label, sizeof(m_label), "%s",
                    m_curveNames[m_curveOrder[current]]);
                  SEKAI_INFO("ANIMATION: %s ended, %s begins",
                    m_curveNames[&curve], m_curveNames[m_curveOrder[current]]);
                  break;
              }
          });

        /*One hand-over: the repeat runs its sequence of curves forever, and the manager
          advances it on every frame of the engine's own update. This layer never touches it
          again.*/
        SekaiEngine::Application::Instance()->Animator().Play(
            std::make_unique<SekaiEngine::Animation::Repeat>(
              MakeCycle(), SekaiEngine::Animation::Repeat::Forever,
              SekaiEngine::Animation::Repeat::Replay));

        /*The first curve is the one running before any boundary has passed, so its name
          goes on screen now; every later refresh comes from the end handler.*/
        snprintf(m_label, sizeof(m_label), "%s", m_curveNames[m_curveOrder.front()]);
    }

    ExampleLayer(const ExampleLayer& layer)
      :m_circle(layer.m_circle),
      m_label(), m_curveNames()
    {
        snprintf(m_label, sizeof(m_label), "%s", layer.m_label);
    }

    ~ExampleLayer()
    {

    }

    void OnEvent(SekaiEngine::Event::Event& event) override
    {
    }

    void OnRender() override
    {
        /*Recorded, not drawn: the command goes into the frame's buffer and is replayed at
          the end of the frame, so every draw below lands in one pass in this same order.*/
        SekaiEngine::Render::RenderProperties props;
        props.Tint = (SekaiEngine::Render::Color)0xff0000ff;
        SekaiEngine::Render::RenderCommand::Record(
            SekaiEngine::Render::MakeCircleCmd(props, m_circle));

        SekaiEngine::Render::RenderProperties labelProps;
        labelProps.Tint = (SekaiEngine::Render::Color)0xffffffff;
        SekaiEngine::Render::RenderCommand::Record(
            SekaiEngine::Render::MakeTextCmd(labelProps, m_label,
              SekaiEngine::Math::Vector2D(LABEL_X, LABEL_Y), "noto-24"));
    }
private:
    /**
     * @brief Build the whole seven-curve cycle, fresh, ready to be wrapped in a repeat
     *
     * @return std::unique_ptr<SekaiEngine::Animation::Node> the sequence of curves
     *
     * @note Built once: the repeat of forever hands the sequence back to its first curve at
     * every boundary, so the sequence does not have to be rebuilt to run again.
     */
    std::unique_ptr<SekaiEngine::Animation::Node> MakeCycle()
    {
        auto sequence = std::make_unique<SekaiEngine::Animation::Sequence>();
        sequence->Add(MakeCurve<SekaiEngine::Animation::Quad<SekaiEngine::Animation::Ease::In>>("Quad In"));
        sequence->Add(MakeCurve<SekaiEngine::Animation::Cubic<SekaiEngine::Animation::Ease::InOut>>("Cubic InOut"));
        sequence->Add(MakeCurve<SekaiEngine::Animation::Sine<SekaiEngine::Animation::Ease::Out>>("Sine Out"));
        sequence->Add(MakeCurve<SekaiEngine::Animation::Expo<SekaiEngine::Animation::Ease::In>>("Expo In"));
        sequence->Add(MakeCurve<SekaiEngine::Animation::Back<SekaiEngine::Animation::Ease::InOut>>("Back InOut"));
        sequence->Add(MakeCurve<SekaiEngine::Animation::Elastic<SekaiEngine::Animation::Ease::Out>>("Elastic Out"));
        sequence->Add(MakeCurve<SekaiEngine::Animation::Bounce<SekaiEngine::Animation::Ease::In>>("Bounce In"));
        return sequence;
    }

    /**
     * @brief Build one curve of the cycle, over the same distance, with a handler of its own
     *
     * @tparam Curve the family and direction, named at the point of use
     * @param name what to put on screen while this curve is the one running
     * @return std::unique_ptr<SekaiEngine::Animation::Node> the curve, ready to join the
     * sequence
     *
     * @note The curve writes the radius only. Its name is refreshed by the manager's end
     * handler when the curve ends, so the label is written once per boundary instead of on
     * every frame. The curve is filed against that name here, and its place in the cycle
     * recorded, while its address is still known.
     */
    template<typename Curve>
    std::unique_ptr<SekaiEngine::Animation::Node> MakeCurve(const char* name)
    {
        std::unique_ptr<SekaiEngine::Animation::Node> curve = std::make_unique<Curve>(
          START_RADIUS, END_RADIUS, SekaiEngine::Timestep(DURATION_SECONDS),
          [this](const float& radius)
          {
              m_circle.Radius = radius;
          });
        m_curveNames[curve.get()] = name;
        m_curveOrder.push_back(curve.get());
        return curve;
    }

    SekaiEngine::Shape::Circle m_circle;
    /*!< The name of the curve being played. A member, not a local in OnRender, because a
      recorded command keeps the pointer it was handed and replays it after that function
      has returned.*/
    char m_label[32];
    /*!< Each curve against the name to put on screen when it ends. Keyed by address, so it
      stays valid while the sequence owns the curves and moves none of them.*/
    std::map<const SekaiEngine::Animation::Node*, const char*> m_curveNames;
    /*!< The curves in the order they run, so the curve after a finished one can be found
      and the last one wraps back to the first.*/
    std::vector<const SekaiEngine::Animation::Node*> m_curveOrder;
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
