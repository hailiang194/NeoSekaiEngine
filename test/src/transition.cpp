#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include "SekaiEngine/Animation/Transitions/Back.h"
#include "SekaiEngine/Animation/Transitions/Bounce.h"
#include "SekaiEngine/Animation/Transitions/Circ.h"
#include "SekaiEngine/Animation/Transitions/Cubic.h"
#include "SekaiEngine/Animation/Transitions/Elastic.h"
#include "SekaiEngine/Animation/Transitions/Expo.h"
#include "SekaiEngine/Animation/Transitions/Linear.h"
#include "SekaiEngine/Animation/Transitions/Quad.h"
#include "SekaiEngine/Animation/Transitions/Quart.h"
#include "SekaiEngine/Animation/Transitions/Quint.h"
#include "SekaiEngine/Animation/Transitions/Sine.h"

/*Below: the Linear transition and the ten catalogue families beside it. All of them are
  header only and need no window and no display, because a transition is fed its elapsed
  time by the caller rather than by the clock.*/

/**
 * @brief Drive a transition and keep every value its handler was called with
 *
 * @note Templated on the transition type so the linear transition and every family are
 * driven by the same helper, rather than each family getting a copy of it.
 */
template<typename TransitionType>
class TransitionRecorder
{
public:
    TransitionRecorder(const float& start, const float& end, const float& seconds,
      const SekaiEngine::Animation::Ease::Mode& mode)
      :m_transition(start, end, SekaiEngine::Timestep(seconds), mode,
        [this](const float& value){ m_values.push_back(value); })
    {
    }

    TransitionRecorder(const float& start, const float& end, const float& seconds)
      :m_transition(start, end, SekaiEngine::Timestep(seconds),
        [this](const float& value){ m_values.push_back(value); })
    {
    }

    /**
     * @brief Advance the transition by one frame of the given length
     *
     */
    void Step(const float& seconds)
    {
        m_transition.Update(SekaiEngine::Timestep(seconds));
    }

    void Start()
    {
        m_transition.Start();
    }

    bool Finished() const
    {
        return m_transition.IsFinish();
    }

    std::vector<float> Values() const
    {
        return m_values;
    }

    float Last() const
    {
        return m_values.empty() ? 0.0f : m_values.back();
    }

private:
    std::vector<float> m_values;
    TransitionType m_transition;
};

using namespace SekaiEngine;
using namespace SekaiEngine::Animation;

/*!< Every direction of every family, in the order the cases below sweep them*/
static const Ease::Mode ALL_MODES[] = {Ease::In, Ease::Out, Ease::InOut};

/**
 * @brief The catalogue's own formulas, written out here independently of the headers
 *
 * @note The point of this file's last group of cases. A test that recomputes a header's
 * expression by calling the header proves nothing; these are transcribed from the
 * catalogue itself, so a wrong exponent or a mis-transformed argument fails here while the
 * header still agrees with itself.
 */
namespace Catalogue
{
    const float C1 = 1.70158f;
    const float C3 = C1 + 1.0f;
    const float C2 = C1 * 1.525f;
    const float C4 = 2.0f * 3.14159265358979323846f / 3.0f;
    const float C5 = 2.0f * 3.14159265358979323846f / 4.5f;
    const float PI = 3.14159265358979323846f;

    float BounceOut(float x)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;
        if(x < 1.0f / d1)
            return n1 * x * x;
        if(x < 2.0f / d1)
        {
            x -= 1.5f / d1;
            return n1 * x * x + 0.75f;
        }
        if(x < 2.5f / d1)
        {
            x -= 2.25f / d1;
            return n1 * x * x + 0.9375f;
        }
        x -= 2.625f / d1;
        return n1 * x * x + 0.984375f;
    }

    float InQuad(float x) { return x * x; }
    float OutQuad(float x) { return 1.0f - (1.0f - x) * (1.0f - x); }
    float InOutQuad(float x) { return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f; }

    float InCubic(float x) { return x * x * x; }
    float OutCubic(float x) { return 1.0f - std::pow(1.0f - x, 3.0f); }
    float InOutCubic(float x) { return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f; }

    float InQuart(float x) { return x * x * x * x; }
    float OutQuart(float x) { return 1.0f - std::pow(1.0f - x, 4.0f); }
    float InOutQuart(float x) { return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 4.0f) / 2.0f; }

    float InQuint(float x) { return x * x * x * x * x; }
    float OutQuint(float x) { return 1.0f - std::pow(1.0f - x, 5.0f); }
    float InOutQuint(float x) { return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 5.0f) / 2.0f; }

    float InSine(float x) { return 1.0f - std::cos(x * PI / 2.0f); }
    float OutSine(float x) { return std::sin(x * PI / 2.0f); }
    float InOutSine(float x) { return -(std::cos(PI * x) - 1.0f) / 2.0f; }

    float InExpo(float x) { return x == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * x - 10.0f); }
    float OutExpo(float x) { return x == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * x); }
    float InOutExpo(float x)
    {
        if(x == 0.0f)
            return 0.0f;
        if(x == 1.0f)
            return 1.0f;
        return x < 0.5f ? std::pow(2.0f, 20.0f * x - 10.0f) / 2.0f : (2.0f - std::pow(2.0f, -20.0f * x + 10.0f)) / 2.0f;
    }

    float InCirc(float x) { return 1.0f - std::sqrt(1.0f - std::pow(x, 2.0f)); }
    float OutCirc(float x) { return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f)); }
    float InOutCirc(float x)
    {
        return x < 0.5f ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * x, 2.0f))) / 2.0f :
            (std::sqrt(1.0f - std::pow(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    float InBack(float x) { return C3 * x * x * x - C1 * x * x; }
    float OutBack(float x) { return 1.0f + C3 * std::pow(x - 1.0f, 3.0f) + C1 * std::pow(x - 1.0f, 2.0f); }
    float InOutBack(float x)
    {
        return x < 0.5f ?
            (std::pow(2.0f * x, 2.0f) * ((C2 + 1.0f) * 2.0f * x - C2)) / 2.0f :
            (std::pow(2.0f * x - 2.0f, 2.0f) * ((C2 + 1.0f) * (x * 2.0f - 2.0f) + C2) + 2.0f) / 2.0f;
    }

    float InElastic(float x)
    {
        if(x == 0.0f)
            return 0.0f;
        if(x == 1.0f)
            return 1.0f;
        return -std::pow(2.0f, 10.0f * x - 10.0f) * std::sin((x * 10.0f - 10.75f) * C4);
    }
    float OutElastic(float x)
    {
        if(x == 0.0f)
            return 0.0f;
        if(x == 1.0f)
            return 1.0f;
        return std::pow(2.0f, -10.0f * x) * std::sin((x * 10.0f - 0.75f) * C4) + 1.0f;
    }
    float InOutElastic(float x)
    {
        if(x == 0.0f)
            return 0.0f;
        if(x == 1.0f)
            return 1.0f;
        return x < 0.5f ?
            -(std::pow(2.0f, 20.0f * x - 10.0f) * std::sin((20.0f * x - 11.125f) * C5)) / 2.0f :
            (std::pow(2.0f, -20.0f * x + 10.0f) * std::sin((20.0f * x - 11.125f) * C5)) / 2.0f + 1.0f;
    }

    float InBounce(float x) { return 1.0f - BounceOut(1.0f - x); }
    float OutBounce(float x) { return BounceOut(x); }
    float InOutBounce(float x)
    {
        return x < 0.5f ? (1.0f - BounceOut(1.0f - 2.0f * x)) / 2.0f : (1.0f + BounceOut(2.0f * x - 1.0f)) / 2.0f;
    }
}

/**
 * @brief Compare one curve against the catalogue across the duration
 *
 * @tparam TransitionType the family under test
 * @param mode the direction under test
 * @param inFamily the catalogue's In curve for this family
 * @param outFamily the catalogue's Out curve
 * @param inOutFamily the catalogue's InOut curve
 */
template<typename TransitionType>
void ExpectMatchesCatalogue(const Ease::Mode& mode,
  float (*inFamily)(float), float (*outFamily)(float), float (*inOutFamily)(float))
{
    TransitionType curve(0.0f, 1.0f, Timestep(1.0f), mode, [](const float&){});

    const float fractions[] = {0.1f, 0.25f, 0.5f, 0.75f, 0.9f};
    for(int index = 0; index < 5; index++)
    {
        const float x = fractions[index];
        float expected = 0.0f;
        if(mode == Ease::In)
            expected = inFamily(x);
        else if(mode == Ease::Out)
            expected = outFamily(x);
        else
            expected = inOutFamily(x);

        /*A tolerance rather than exact equality: the catalogue evaluates in double, so the
          last bit of the two can differ even when the algebra agrees.*/
        EXPECT_NEAR(expected, curve.GetProgressValue(x), 1e-5f) << " at fraction " << x;
    }
}

/**
 * @brief Check the two ends of one curve in one direction, and its mid-point
 *
 * @tparam TransitionType the family under test
 * @param mode the direction under test
 */
template<typename TransitionType>
void ExpectExactEnds(const Ease::Mode& mode)
{
    TransitionType curve(0.0f, 1.0f, Timestep(1.0f), mode, [](const float&){});

    EXPECT_EQ(0.0f, curve.GetProgressValue(0.0f));
    EXPECT_EQ(1.0f, curve.GetProgressValue(1.0f));
}

TEST(EngineTest, TestStartReportsTheStartValue)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);

    recorder.Start();

    ASSERT_EQ(1U, recorder.Values().size());
    EXPECT_FLOAT_EQ(30.0f, recorder.Last());
}

TEST(EngineTest, TestHalfTheDurationCoversHalfTheDistance)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);

    recorder.Start();
    recorder.Step(3.0f);

    EXPECT_FLOAT_EQ(50.0f, recorder.Last());
}

TEST(EngineTest, TestTheEndValueIsReachedOnTheDuration)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);

    recorder.Start();
    for(int frame = 0; frame < 6; frame++)
    {
        recorder.Step(1.0f);
    }

    EXPECT_FLOAT_EQ(70.0f, recorder.Last());
}

TEST(EngineTest, TestAnOvershootingStepIsClampedToTheEndValue)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);

    recorder.Start();
    /*One frame longer than the whole transition, so the elapsed time is capped at the
      duration instead of running past the end value.*/
    recorder.Step(60.0f);

    EXPECT_FLOAT_EQ(70.0f, recorder.Last());
}

TEST(EngineTest, TestAFinishedTransitionStopsReporting)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);

    recorder.Start();
    recorder.Step(6.0f);
    int reported = static_cast<int>(recorder.Values().size());
    recorder.Step(1.0f);
    recorder.Step(1.0f);

    EXPECT_EQ(reported, static_cast<int>(recorder.Values().size()));
}

TEST(EngineTest, TestStartRewindsAFinishedTransition)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);

    recorder.Start();
    recorder.Step(6.0f);
    EXPECT_FLOAT_EQ(70.0f, recorder.Last());

    recorder.Start();
    EXPECT_FLOAT_EQ(30.0f, recorder.Last());

    recorder.Step(3.0f);
    EXPECT_FLOAT_EQ(50.0f, recorder.Last());
}

TEST(EngineTest, TestAZeroLengthTransitionJumpsStraightToItsEndValue)
{
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 0.0f);

    /*There is no duration to divide the elapsed time by, so the value must land on its
      end rather than on a NaN, and it must be there from the start.*/
    recorder.Start();
    EXPECT_FLOAT_EQ(70.0f, recorder.Last());

    recorder.Step(1.0f);
    EXPECT_FLOAT_EQ(70.0f, recorder.Last());
}

TEST(EngineTest, TestADecreasingTransitionRunsDownwards)
{
    TransitionRecorder<Linear> recorder(70.0f, 30.0f, 6.0f);

    recorder.Start();
    recorder.Step(3.0f);

    EXPECT_FLOAT_EQ(50.0f, recorder.Last());
}

TEST(EngineTest, TestEveryCurveLandsExactlyOnItsEnds)
{
    /*Every family, every direction: the start of a duration is exactly zero and the end is
      exactly one, on raw float equality rather than a tolerance. A curve whose algebra
      leaves residue at either end fails here even though it looks right on a plot.*/
    for(int index = 0; index < 3; index++)
    {
        const Ease::Mode mode = ALL_MODES[index];

        ExpectExactEnds<Quad>(mode);
        ExpectExactEnds<Cubic>(mode);
        ExpectExactEnds<Quart>(mode);
        ExpectExactEnds<Quint>(mode);
        ExpectExactEnds<Sine>(mode);
        ExpectExactEnds<Expo>(mode);
        ExpectExactEnds<Circ>(mode);
        ExpectExactEnds<Back>(mode);
        ExpectExactEnds<Elastic>(mode);
        ExpectExactEnds<Bounce>(mode);
    }
}

TEST(EngineTest, TestEveryCurveReachesItsEndValueAndStops)
{
    /*Driven through the engine's own transition rather than evaluated by hand, so this
      covers the requirement that a curve is usable wherever the linear transition is: the
      last reported value is the end value exactly, and nothing is reported after it.*/
    for(int index = 0; index < 3; index++)
    {
        const Ease::Mode mode = ALL_MODES[index];

        TransitionRecorder<Quad> quad(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Cubic> cubic(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Quart> quart(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Quint> quint(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Sine> sine(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Expo> expo(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Circ> circ(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Back> back(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Elastic> elastic(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Bounce> bounce(30.0f, 70.0f, 1.0f, mode);

        for(int frame = 0; frame < 20; frame++)
        {
            quad.Step(0.1f);
            cubic.Step(0.1f);
            quart.Step(0.1f);
            quint.Step(0.1f);
            sine.Step(0.1f);
            expo.Step(0.1f);
            circ.Step(0.1f);
            back.Step(0.1f);
            elastic.Step(0.1f);
            bounce.Step(0.1f);
        }

        EXPECT_FLOAT_EQ(70.0f, quad.Last());
        EXPECT_FLOAT_EQ(70.0f, cubic.Last());
        EXPECT_FLOAT_EQ(70.0f, quart.Last());
        EXPECT_FLOAT_EQ(70.0f, quint.Last());
        EXPECT_FLOAT_EQ(70.0f, sine.Last());
        EXPECT_FLOAT_EQ(70.0f, expo.Last());
        EXPECT_FLOAT_EQ(70.0f, circ.Last());
        EXPECT_FLOAT_EQ(70.0f, back.Last());
        EXPECT_FLOAT_EQ(70.0f, elastic.Last());
        EXPECT_FLOAT_EQ(70.0f, bounce.Last());

        const int reported = static_cast<int>(quad.Values().size());
        quad.Step(0.1f);
        EXPECT_EQ(reported, static_cast<int>(quad.Values().size()));
    }
}

TEST(EngineTest, TestANonOvershootingCurveStaysBetweenItsValues)
{
    /*The eight families the catalogue does not define to overshoot, in all three
      directions and travelling both upwards and downwards: nothing they report may leave
      the two values they run between.*/
    for(int index = 0; index < 3; index++)
    {
        const Ease::Mode mode = ALL_MODES[index];

        const float low = 10.0f;
        const float high = 90.0f;
        const float up[][2] = {{low, high}, {high, low}};

        for(int travel = 0; travel < 2; travel++)
        {
            const float start = up[travel][0];
            const float end = up[travel][1];
            const float floorValue = std::min(start, end);
            const float ceiling = std::max(start, end);

            Quad quad(start, end, Timestep(1.0f), mode, [](const float&){});
            Cubic cubic(start, end, Timestep(1.0f), mode, [](const float&){});
            Quart quart(start, end, Timestep(1.0f), mode, [](const float&){});
            Quint quint(start, end, Timestep(1.0f), mode, [](const float&){});
            Sine sine(start, end, Timestep(1.0f), mode, [](const float&){});
            Expo expo(start, end, Timestep(1.0f), mode, [](const float&){});
            Circ circ(start, end, Timestep(1.0f), mode, [](const float&){});
            Bounce bounce(start, end, Timestep(1.0f), mode, [](const float&){});

            for(int step = 0; step <= 200; step++)
            {
                const float ratio = step / 200.0f;

                const float reported[] = {
                    start + (end - start) * quad.GetProgressValue(ratio),
                    start + (end - start) * cubic.GetProgressValue(ratio),
                    start + (end - start) * quart.GetProgressValue(ratio),
                    start + (end - start) * quint.GetProgressValue(ratio),
                    start + (end - start) * sine.GetProgressValue(ratio),
                    start + (end - start) * expo.GetProgressValue(ratio),
                    start + (end - start) * circ.GetProgressValue(ratio),
                    start + (end - start) * bounce.GetProgressValue(ratio)
                };

                for(int family = 0; family < 8; family++)
                {
                    EXPECT_TRUE(std::isfinite(reported[family]));
                    EXPECT_GE(reported[family], floorValue);
                    EXPECT_LE(reported[family], ceiling);
                }
            }
        }
    }
}

/*!< How far past its ends each overshooting family travels, per direction. Where the
  catalogue's In curve dips below the start, and its Out curve sails past the end, is the
  whole difference between the two directions: the shape is spent at different ends, so it
  overshoots in opposite directions.*/
static const bool OVERSHOOTS_PAST_END[] = {false, true, true};
static const bool OVERSHOOTS_PAST_START[] = {true, false, true};

TEST(EngineTest, TestBackOvershootsAndStillLandsOnItsEnd)
{
    /*Both halves matter, and neither satisfies the other: an overshoot that is never
      removed fails the second, and one that was never there fails the first.*/
    for(int index = 0; index < 3; index++)
    {
        const Ease::Mode mode = ALL_MODES[index];

        Back back(0.0f, 1.0f, Timestep(1.0f), mode, [](const float&){});

        float highest = -1e30f;
        float lowest = 1e30f;
        for(int step = 0; step <= 200; step++)
        {
            const float value = back.GetProgressValue(step / 200.0f);
            highest = std::max(highest, value);
            lowest = std::min(lowest, value);
        }

        /*Where the shape is spent decides which way it overshoots: In leaves in the wrong
          direction and never passes the end, Out arrives early and never returns below the
          start, and InOut does both.*/
        if(OVERSHOOTS_PAST_END[index])
            EXPECT_GT(highest, 1.0f) << "Back in mode " << index << " never passed its end";
        else
            EXPECT_LE(highest, 1.0f);

        if(OVERSHOOTS_PAST_START[index])
            EXPECT_LT(lowest, 0.0f) << "Back in mode " << index << " never went back before its start";
        else
            EXPECT_GE(lowest, 0.0f);

        EXPECT_EQ(0.0f, back.GetProgressValue(0.0f));
        EXPECT_EQ(1.0f, back.GetProgressValue(1.0f));
    }
}

TEST(EngineTest, TestElasticOvershootsBothWaysAndStillLandsOnItsEnd)
{
    for(int index = 0; index < 3; index++)
    {
        const Ease::Mode mode = ALL_MODES[index];

        Elastic elastic(0.0f, 1.0f, Timestep(1.0f), mode, [](const float&){});

        float highest = -1e30f;
        float lowest = 1e30f;
        for(int step = 0; step <= 400; step++)
        {
            const float value = elastic.GetProgressValue(step / 400.0f);
            EXPECT_TRUE(std::isfinite(value));
            highest = std::max(highest, value);
            lowest = std::min(lowest, value);
        }

        if(OVERSHOOTS_PAST_END[index])
            EXPECT_GT(highest, 1.0f) << "Elastic in mode " << index << " never passed its end";

        if(OVERSHOOTS_PAST_START[index])
            EXPECT_LT(lowest, 0.0f) << "Elastic in mode " << index << " never dipped back before its start";

        EXPECT_EQ(0.0f, elastic.GetProgressValue(0.0f));
        EXPECT_EQ(1.0f, elastic.GetProgressValue(1.0f));
    }
}

TEST(EngineTest, TestAnOvershootingTransitionSettlesOntoItsEndInsteadOfDiverging)
{
    /*Driven through the engine's transition, and stepped well past the end of its duration,
      because that is the situation the requirement is about: a game whose frame is longer
      than the transition must get the end value and then silence, never a value that keeps
      travelling. The base caps the elapsed time at the duration, so the curve is asked
      about a ratio of one at most however long the frame was.*/
    for(int index = 0; index < 3; index++)
    {
        const Ease::Mode mode = ALL_MODES[index];

        TransitionRecorder<Back> back(30.0f, 70.0f, 1.0f, mode);
        TransitionRecorder<Elastic> elastic(30.0f, 70.0f, 1.0f, mode);

        for(int frame = 0; frame < 40; frame++)
        {
            /*Every frame is ten times longer than the whole transition, so the run is past
              its end from the second frame onwards.*/
            back.Step(10.0f);
            elastic.Step(10.0f);

            EXPECT_TRUE(std::isfinite(back.Last()));
            EXPECT_TRUE(std::isfinite(elastic.Last()));
            EXPECT_FLOAT_EQ(70.0f, back.Last());
            EXPECT_FLOAT_EQ(70.0f, elastic.Last());
        }

        /*Once past its end it stops reporting, so an overshooting curve cannot keep
          pushing a value at the game.*/
        const int reported = static_cast<int>(back.Values().size());
        back.Step(10.0f);
        elastic.Step(10.0f);
        EXPECT_EQ(reported, static_cast<int>(back.Values().size()));
    }
}

TEST(EngineTest, TestOutIsTheComplementOfIn)
{
    /*A transition's Out direction is its In direction on the remaining distance, so the two
      halves of the journey add to the whole: out(x) equals one minus in(1-x). Checked the
      other way round, one minus in(x), it looks right at both ends and is wrong everywhere
      in between, which is what this catches. The two overshooting families are excluded:
      they deliberately report values outside 0 to 1, so the complement of one is not a
      fraction any more.*/
    Quad inQuad(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Quad outQuad(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Cubic inCubic(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Cubic outCubic(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Quart inQuart(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Quart outQuart(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Quint inQuint(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Quint outQuint(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Sine inSine(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Sine outSine(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Expo inExpo(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Expo outExpo(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Circ inCirc(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Circ outCirc(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Bounce inBounce(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Bounce outBounce(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});

    for(int step = 0; step <= 10; step++)
    {
        const float x = step / 10.0f;
        const float remaining = 1.0f - x;

        EXPECT_NEAR(1.0f - inQuad.GetProgressValue(remaining), outQuad.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inCubic.GetProgressValue(remaining), outCubic.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inQuart.GetProgressValue(remaining), outQuart.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inQuint.GetProgressValue(remaining), outQuint.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inSine.GetProgressValue(remaining), outSine.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inExpo.GetProgressValue(remaining), outExpo.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inCirc.GetProgressValue(remaining), outCirc.GetProgressValue(x), 1e-5f) << "x = " << x;
        EXPECT_NEAR(1.0f - inBounce.GetProgressValue(remaining), outBounce.GetProgressValue(x), 1e-5f) << "x = " << x;
    }
}

TEST(EngineTest, TestInOutIsAtItsMidpointHalfwayThrough)
{
    /*The InOut direction only, and deliberately so: this is the direction defined to split
      its shape across both ends, so its halfway value is the midpoint by definition. The In
      and Out directions are not, and for the two overshooting families they are far from
      it — that is the overshoot, checked in the cases above.*/
    const Ease::Mode mode = Ease::InOut;

    {
        TransitionRecorder<Quad> quad(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Cubic> cubic(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Quart> quart(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Quint> quint(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Sine> sine(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Expo> expo(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Circ> circ(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Back> back(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Elastic> elastic(20.0f, 80.0f, 1.0f, mode);
        TransitionRecorder<Bounce> bounce(20.0f, 80.0f, 1.0f, mode);

        quad.Step(0.5f);
        cubic.Step(0.5f);
        quart.Step(0.5f);
        quint.Step(0.5f);
        sine.Step(0.5f);
        expo.Step(0.5f);
        circ.Step(0.5f);
        back.Step(0.5f);
        elastic.Step(0.5f);
        bounce.Step(0.5f);

        EXPECT_FLOAT_EQ(50.0f, quad.Last());
        EXPECT_FLOAT_EQ(50.0f, cubic.Last());
        EXPECT_FLOAT_EQ(50.0f, quart.Last());
        EXPECT_FLOAT_EQ(50.0f, quint.Last());
        EXPECT_FLOAT_EQ(50.0f, sine.Last());
        EXPECT_FLOAT_EQ(50.0f, expo.Last());
        EXPECT_FLOAT_EQ(50.0f, circ.Last());
        EXPECT_FLOAT_EQ(50.0f, back.Last());
        EXPECT_FLOAT_EQ(50.0f, elastic.Last());
        EXPECT_FLOAT_EQ(50.0f, bounce.Last());
    }
}

TEST(EngineTest, TestTheThreeDirectionsAreNotTheSameCurve)
{
    /*Each direction spends the shape somewhere different, so the same family in the same
      three directions must not report the same value part way through.*/
    Quad quadIn(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Quad quadOut(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Quad quadInOut(0.0f, 1.0f, Timestep(1.0f), Ease::InOut, [](const float&){});

    Elastic elasticIn(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});
    Elastic elasticOut(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Elastic elasticInOut(0.0f, 1.0f, Timestep(1.0f), Ease::InOut, [](const float&){});

    for(int step = 1; step < 10; step++)
    {
        const float at = step / 10.0f;

        EXPECT_NE(quadIn.GetProgressValue(at), quadOut.GetProgressValue(at)) << "at " << at;
        EXPECT_NE(quadIn.GetProgressValue(at), quadInOut.GetProgressValue(at)) << "at " << at;
        EXPECT_NE(quadOut.GetProgressValue(at), quadInOut.GetProgressValue(at)) << "at " << at;

        EXPECT_NE(elasticIn.GetProgressValue(at), elasticOut.GetProgressValue(at)) << "at " << at;
        EXPECT_NE(elasticIn.GetProgressValue(at), elasticInOut.GetProgressValue(at)) << "at " << at;
        EXPECT_NE(elasticOut.GetProgressValue(at), elasticInOut.GetProgressValue(at)) << "at " << at;
    }
}

TEST(EngineTest, TestACurvesShapeDoesNotDependOnItsDuration)
{
    /*The same fraction of two different durations over the same distance reports the same
      value: a curve is a function of elapsed proportion, never of elapsed seconds.*/
    Quad quick(0.0f, 100.0f, Timestep(1.0f), Ease::InOut, [](const float&){});
    Quad slow(0.0f, 100.0f, Timestep(10.0f), Ease::InOut, [](const float&){});
    Elastic quickElastic(0.0f, 100.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Elastic slowElastic(0.0f, 100.0f, Timestep(10.0f), Ease::Out, [](const float&){});

    for(int step = 0; step <= 20; step++)
    {
        const float ratio = step / 20.0f;
        EXPECT_FLOAT_EQ(quick.GetProgressValue(ratio), slow.GetProgressValue(ratio));
        EXPECT_FLOAT_EQ(quickElastic.GetProgressValue(ratio), slowElastic.GetProgressValue(ratio));
    }
}

TEST(EngineTest, TestACurveIsTheSameValueEveryTimeItIsEvaluated)
{
    Back back(0.0f, 1.0f, Timestep(1.0f), Ease::Out, [](const float&){});
    Elastic elastic(0.0f, 1.0f, Timestep(1.0f), Ease::In, [](const float&){});

    for(int step = 0; step <= 20; step++)
    {
        const float ratio = step / 20.0f;
        EXPECT_EQ(back.GetProgressValue(ratio), back.GetProgressValue(ratio));
        EXPECT_EQ(elastic.GetProgressValue(ratio), elastic.GetProgressValue(ratio));
    }
}

TEST(EngineTest, TestACurveCanBePlayedAgainAfterItFinishes)
{
    /*The InOut direction, whose halfway value is the midpoint, so a replay is checked
      against a value the curve is known to produce. An overshooting family's In curve is
      deliberately below its midpoint at that point, which is what the overshoot cases
      above already cover.*/
    TransitionRecorder<Back> back(20.0f, 80.0f, 1.0f, Ease::InOut);
    TransitionRecorder<Elastic> elastic(20.0f, 80.0f, 1.0f, Ease::InOut);

    back.Step(1.0f);
    elastic.Step(1.0f);
    EXPECT_FLOAT_EQ(80.0f, back.Last());
    EXPECT_FLOAT_EQ(80.0f, elastic.Last());

    back.Start();
    elastic.Start();
    EXPECT_FLOAT_EQ(20.0f, back.Last());
    EXPECT_FLOAT_EQ(20.0f, elastic.Last());

    back.Step(0.5f);
    elastic.Step(0.5f);
    EXPECT_FLOAT_EQ(50.0f, back.Last());
    EXPECT_FLOAT_EQ(50.0f, elastic.Last());

    /*And every direction reports its start again when rewound, overshooting or not.*/
    for(int index = 0; index < 3; index++)
    {
        TransitionRecorder<Back> rewoundBack(20.0f, 80.0f, 1.0f, ALL_MODES[index]);
        TransitionRecorder<Elastic> rewoundElastic(20.0f, 80.0f, 1.0f, ALL_MODES[index]);

        rewoundBack.Step(1.0f);
        rewoundElastic.Step(1.0f);
        EXPECT_FLOAT_EQ(80.0f, rewoundBack.Last());
        EXPECT_FLOAT_EQ(80.0f, rewoundElastic.Last());

        rewoundBack.Start();
        rewoundElastic.Start();
        EXPECT_FLOAT_EQ(20.0f, rewoundBack.Last());
        EXPECT_FLOAT_EQ(20.0f, rewoundElastic.Last());
    }
}

TEST(EngineTest, TestAZeroLengthCurveTransitionJumpsStraightToItsEndValue)
{
    /*The base has no duration to divide the elapsed time by for any family, so none of them
      may produce a NaN here.*/
    for(int index = 0; index < 3; index++)
    {
        TransitionRecorder<Quad> quad(30.0f, 70.0f, 0.0f, ALL_MODES[index]);
        TransitionRecorder<Elastic> elastic(30.0f, 70.0f, 0.0f, ALL_MODES[index]);
        TransitionRecorder<Bounce> bounce(30.0f, 70.0f, 0.0f, ALL_MODES[index]);

        quad.Start();
        elastic.Start();
        bounce.Start();

        EXPECT_FLOAT_EQ(70.0f, quad.Last());
        EXPECT_FLOAT_EQ(70.0f, elastic.Last());
        EXPECT_FLOAT_EQ(70.0f, bounce.Last());
    }
}

TEST(EngineTest, TestBounceReversesDirection)
{
    for(int index = 0; index < 3; index++)
    {
        Bounce bounce(0.0f, 1.0f, Timestep(1.0f), ALL_MODES[index], [](const float&){});

        int reversals = 0;
        float rising = bounce.GetProgressValue(0.0f) < bounce.GetProgressValue(0.01f);
        for(int step = 1; step < 400; step++)
        {
            const bool nowRising = bounce.GetProgressValue(step / 400.0f) > bounce.GetProgressValue((step - 1) / 400.0f);
            if(nowRising != rising)
            {
                reversals++;
                rising = nowRising;
            }
        }

        EXPECT_GE(reversals, 2) << "Bounce in mode " << index << " never rebounded";
    }
}

TEST(EngineTest, TestQuadMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Quad>(ALL_MODES[index], Catalogue::InQuad, Catalogue::OutQuad, Catalogue::InOutQuad);
}

TEST(EngineTest, TestCubicMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Cubic>(ALL_MODES[index], Catalogue::InCubic, Catalogue::OutCubic, Catalogue::InOutCubic);
}

TEST(EngineTest, TestQuartMatchesTheCatalogue)
{
    /*Swapping this family's exponent for Cubic's fails here and nowhere else in the file:
      every other case either passes for both powers or checks a property both share.*/
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Quart>(ALL_MODES[index], Catalogue::InQuart, Catalogue::OutQuart, Catalogue::InOutQuart);
}

TEST(EngineTest, TestQuintMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Quint>(ALL_MODES[index], Catalogue::InQuint, Catalogue::OutQuint, Catalogue::InOutQuint);
}

TEST(EngineTest, TestSineMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Sine>(ALL_MODES[index], Catalogue::InSine, Catalogue::OutSine, Catalogue::InOutSine);
}

TEST(EngineTest, TestExpoMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Expo>(ALL_MODES[index], Catalogue::InExpo, Catalogue::OutExpo, Catalogue::InOutExpo);
}

TEST(EngineTest, TestCircMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Circ>(ALL_MODES[index], Catalogue::InCirc, Catalogue::OutCirc, Catalogue::InOutCirc);
}

TEST(EngineTest, TestBackMatchesTheCatalogueAtItsFixedOvershoot)
{
    /*The catalogue's own overshoot, transcribed here rather than read out of the header,
      so a changed constant in the header fails instead of agreeing with itself.*/
    EXPECT_FLOAT_EQ(1.70158f, BACK_OVERSHOOT);

    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Back>(ALL_MODES[index], Catalogue::InBack, Catalogue::OutBack, Catalogue::InOutBack);
}

TEST(EngineTest, TestElasticMatchesTheCatalogueAtItsFixedPeriod)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Elastic>(ALL_MODES[index], Catalogue::InElastic, Catalogue::OutElastic, Catalogue::InOutElastic);
}

TEST(EngineTest, TestBounceMatchesTheCatalogue)
{
    for(int index = 0; index < 3; index++)
        ExpectMatchesCatalogue<Bounce>(ALL_MODES[index], Catalogue::InBounce, Catalogue::OutBounce, Catalogue::InOutBounce);
}

TEST(EngineTest, TestTheLinearTransitionStillBehavesAsItDid)
{
    /*The catalogue's Linear family is what this transition already was, and this change
      adds nothing to it, so its shape is pinned here beside the ten that are new.*/
    Linear linear(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

    for(int step = 0; step <= 20; step++)
        EXPECT_FLOAT_EQ(step / 20.0f, linear.GetProgressValue(step / 20.0f));

    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 6.0f);
    recorder.Start();
    recorder.Step(3.0f);
    EXPECT_FLOAT_EQ(50.0f, recorder.Last());
}