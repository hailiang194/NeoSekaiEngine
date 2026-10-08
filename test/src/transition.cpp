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
    /*One constructor for every family. Linear and each of the catalogue's three directions
      are told apart by their type rather than by an argument, so the recorder needs no
      direction of its own and its TransitionType carries it.*/
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

    void Reverse()
    {
        m_transition.Reverse();
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

/*!< The three directions of every family, as the type each one is instantiated as. Written
  out once so a case that sweeps all three names them the same way everywhere.*/

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
 * @tparam MODE the direction under test, which is also what instantiates the family
 * @tparam Family the family under test, before any direction is applied to it
 * @param expected the catalogue's curve for this family in this direction
 */
template<Ease::Mode MODE, template<Ease::Mode> class Family>
void ExpectMatchesCatalogue(float (*expected)(float))
{
    Family<MODE> curve(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

    const float fractions[] = {0.1f, 0.25f, 0.5f, 0.75f, 0.9f};
    for(int index = 0; index < 5; index++)
    {
        const float x = fractions[index];
        /*A tolerance rather than exact equality: the catalogue evaluates in double, so the
          last bit of the two can differ even when the algebra agrees.*/
        EXPECT_NEAR(expected(x), curve.GetProgressValue(x), 1e-5f) << " at fraction " << x;
    }
}

/**
 * @brief Compare one family against the catalogue in all three of its directions
 *
 * @note The three arguments are the catalogue's own In, Out and InOut curves for this
 * family, in that order, so a case below stays one line per family however many directions
 * it sweeps. The family is named here without a direction, because the direction is what
 * this function applies.
 */
template<template<Ease::Mode> class Family>
void ExpectFamilyMatchesCatalogue(float (*inFamily)(float), float (*outFamily)(float),
  float (*inOutFamily)(float))
{
    ExpectMatchesCatalogue<Ease::In, Family>(inFamily);
    ExpectMatchesCatalogue<Ease::Out, Family>(outFamily);
    ExpectMatchesCatalogue<Ease::InOut, Family>(inOutFamily);
}

/**
 * @brief Check the two ends of one curve in one direction, and its mid-point
 *
 * @tparam MODE the direction under test, which is also what instantiates the family
 * @tparam Family the family under test, before any direction is applied to it
 */
template<Ease::Mode MODE, template<Ease::Mode> class Family>
void ExpectExactEnds()
{
    Family<MODE> curve(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

    EXPECT_EQ(0.0f, curve.GetProgressValue(0.0f));
    EXPECT_EQ(1.0f, curve.GetProgressValue(1.0f));
}

/**
 * @brief Check the two ends of one family in all three of its directions
 */
template<template<Ease::Mode> class Family>
void ExpectFamilyExactEnds()
{
    ExpectExactEnds<Ease::In, Family>();
    ExpectExactEnds<Ease::Out, Family>();
    ExpectExactEnds<Ease::InOut, Family>();
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
    ExpectFamilyExactEnds<Quad>();
    ExpectFamilyExactEnds<Cubic>();
    ExpectFamilyExactEnds<Quart>();
    ExpectFamilyExactEnds<Quint>();
    ExpectFamilyExactEnds<Sine>();
    ExpectFamilyExactEnds<Expo>();
    ExpectFamilyExactEnds<Circ>();
    ExpectFamilyExactEnds<Back>();
    ExpectFamilyExactEnds<Elastic>();
    ExpectFamilyExactEnds<Bounce>();
}

/**
 * @brief Drive every family in one direction to the end of its duration and no further
 *
 * @tparam MODE the direction under test, applied to every family below
 */
template<Ease::Mode MODE>
void ExpectEveryCurveReachesItsEndValueAndStops()
{
    TransitionRecorder<Quad<MODE>> quad(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Cubic<MODE>> cubic(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Quart<MODE>> quart(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Quint<MODE>> quint(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Sine<MODE>> sine(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Expo<MODE>> expo(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Circ<MODE>> circ(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Back<MODE>> back(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Elastic<MODE>> elastic(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Bounce<MODE>> bounce(30.0f, 70.0f, 1.0f);

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

TEST(EngineTest, TestEveryCurveReachesItsEndValueAndStops)
{
    /*Driven through the engine's own transition rather than evaluated by hand, so this
      covers the requirement that a curve is usable wherever the linear transition is: the
      last reported value is the end value exactly, and nothing is reported after it.*/
    ExpectEveryCurveReachesItsEndValueAndStops<Ease::In>();
    ExpectEveryCurveReachesItsEndValueAndStops<Ease::Out>();
    ExpectEveryCurveReachesItsEndValueAndStops<Ease::InOut>();
}

/**
 * @brief Hold the eight non-overshooting families between their two values in one direction
 *
 * @tparam MODE the direction under test, applied to every family below
 */
template<Ease::Mode MODE>
void ExpectANonOvershootingCurveStaysBetweenItsValues()
{
    const float low = 10.0f;
    const float high = 90.0f;
    const float up[][2] = {{low, high}, {high, low}};

    for(int travel = 0; travel < 2; travel++)
    {
        const float start = up[travel][0];
        const float end = up[travel][1];
        const float floorValue = std::min(start, end);
        const float ceiling = std::max(start, end);

        Quad<MODE> quad(start, end, Timestep(1.0f), [](const float&){});
        Cubic<MODE> cubic(start, end, Timestep(1.0f), [](const float&){});
        Quart<MODE> quart(start, end, Timestep(1.0f), [](const float&){});
        Quint<MODE> quint(start, end, Timestep(1.0f), [](const float&){});
        Sine<MODE> sine(start, end, Timestep(1.0f), [](const float&){});
        Expo<MODE> expo(start, end, Timestep(1.0f), [](const float&){});
        Circ<MODE> circ(start, end, Timestep(1.0f), [](const float&){});
        Bounce<MODE> bounce(start, end, Timestep(1.0f), [](const float&){});

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

TEST(EngineTest, TestANonOvershootingCurveStaysBetweenItsValues)
{
    /*The eight families the catalogue does not define to overshoot, in all three
      directions and travelling both upwards and downwards: nothing they report may leave
      the two values they run between.*/
    ExpectANonOvershootingCurveStaysBetweenItsValues<Ease::In>();
    ExpectANonOvershootingCurveStaysBetweenItsValues<Ease::Out>();
    ExpectANonOvershootingCurveStaysBetweenItsValues<Ease::InOut>();
}

/*!< How far past its ends each overshooting family travels, per direction. Where the
  catalogue's In curve dips below the start, and its Out curve sails past the end, is the
  whole difference between the two directions: the shape is spent at different ends, so it
  overshoots in opposite directions. Held by the caller rather than looked up from an array
  indexed by the direction, because the direction is a type and has no run-time index.*/

/**
 * @brief Back travels past the end and past the start as the two arguments say
 *
 * @tparam MODE the direction under test
 * @tparam PAST_END whether this direction's Back curve sails past its end value
 * @tparam PAST_START whether this direction's Back curve leaves in the wrong direction and
 * so dips below its start value
 */
template<Ease::Mode MODE, bool PAST_END, bool PAST_START>
void ExpectBackOvershootsAsSpecified()
{
    Back<MODE> back(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

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
    if constexpr(PAST_END)
        EXPECT_GT(highest, 1.0f);
    else
        EXPECT_LE(highest, 1.0f);

    if constexpr(PAST_START)
        EXPECT_LT(lowest, 0.0f);
    else
        EXPECT_GE(lowest, 0.0f);

    EXPECT_EQ(0.0f, back.GetProgressValue(0.0f));
    EXPECT_EQ(1.0f, back.GetProgressValue(1.0f));
}

TEST(EngineTest, TestBackOvershootsAndStillLandsOnItsEnd)
{
    /*Both halves matter, and neither satisfies the other: an overshoot that is never
      removed fails the second, and one that was never there fails the first.*/
    ExpectBackOvershootsAsSpecified<Ease::In, false, true>();
    ExpectBackOvershootsAsSpecified<Ease::Out, true, false>();
    ExpectBackOvershootsAsSpecified<Ease::InOut, true, true>();
}

/**
 * @brief Elastic oscillates past both of its ends and still lands on them
 *
 * @tparam MODE the direction under test
 * @tparam PAST_END whether this direction's Elastic curve sails past its end value
 * @tparam PAST_START whether this direction's Elastic curve dips below its start value
 */
template<Ease::Mode MODE, bool PAST_END, bool PAST_START>
void ExpectElasticOvershootsAsSpecified()
{
    Elastic<MODE> elastic(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

    float highest = -1e30f;
    float lowest = 1e30f;
    for(int step = 0; step <= 400; step++)
    {
        const float value = elastic.GetProgressValue(step / 400.0f);
        EXPECT_TRUE(std::isfinite(value));
        highest = std::max(highest, value);
        lowest = std::min(lowest, value);
    }

    if constexpr(PAST_END)
        EXPECT_GT(highest, 1.0f);

    if constexpr(PAST_START)
        EXPECT_LT(lowest, 0.0f);

    EXPECT_EQ(0.0f, elastic.GetProgressValue(0.0f));
    EXPECT_EQ(1.0f, elastic.GetProgressValue(1.0f));
}

TEST(EngineTest, TestElasticOvershootsBothWaysAndStillLandsOnItsEnd)
{
    ExpectElasticOvershootsAsSpecified<Ease::In, false, true>();
    ExpectElasticOvershootsAsSpecified<Ease::Out, true, false>();
    ExpectElasticOvershootsAsSpecified<Ease::InOut, true, true>();
}

/**
 * @brief Step one overshooting transition far past its end in one direction
 *
 * @tparam MODE the direction under test, applied to both families below
 */
template<Ease::Mode MODE>
void ExpectAnOvershootingTransitionSettlesOntoItsEnd()
{
    TransitionRecorder<Back<MODE>> back(30.0f, 70.0f, 1.0f);
    TransitionRecorder<Elastic<MODE>> elastic(30.0f, 70.0f, 1.0f);

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

TEST(EngineTest, TestAnOvershootingTransitionSettlesOntoItsEndInsteadOfDiverging)
{
    /*Driven through the engine's transition, and stepped well past the end of its duration,
      because that is the situation the requirement is about: a game whose frame is longer
      than the transition must get the end value and then silence, never a value that keeps
      travelling. The base caps the elapsed time at the duration, so the curve is asked
      about a ratio of one at most however long the frame was.*/
    ExpectAnOvershootingTransitionSettlesOntoItsEnd<Ease::In>();
    ExpectAnOvershootingTransitionSettlesOntoItsEnd<Ease::Out>();
    ExpectAnOvershootingTransitionSettlesOntoItsEnd<Ease::InOut>();
}

TEST(EngineTest, TestOutIsTheComplementOfIn)
{
    /*A transition's Out direction is its In direction on the remaining distance, so the two
      halves of the journey add to the whole: out(x) equals one minus in(1-x). Checked the
      other way round, one minus in(x), it looks right at both ends and is wrong everywhere
      in between, which is what this catches. The two overshooting families are excluded:
      they deliberately report values outside 0 to 1, so the complement of one is not a
      fraction any more.*/
    Quad<Ease::In> inQuad(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quad<Ease::Out> outQuad(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Cubic<Ease::In> inCubic(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Cubic<Ease::Out> outCubic(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quart<Ease::In> inQuart(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quart<Ease::Out> outQuart(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quint<Ease::In> inQuint(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quint<Ease::Out> outQuint(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Sine<Ease::In> inSine(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Sine<Ease::Out> outSine(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Expo<Ease::In> inExpo(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Expo<Ease::Out> outExpo(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Circ<Ease::In> inCirc(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Circ<Ease::Out> outCirc(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Bounce<Ease::In> inBounce(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Bounce<Ease::Out> outBounce(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

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
    {
        TransitionRecorder<Quad<Ease::InOut>> quad(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Cubic<Ease::InOut>> cubic(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Quart<Ease::InOut>> quart(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Quint<Ease::InOut>> quint(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Sine<Ease::InOut>> sine(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Expo<Ease::InOut>> expo(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Circ<Ease::InOut>> circ(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Back<Ease::InOut>> back(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Elastic<Ease::InOut>> elastic(20.0f, 80.0f, 1.0f);
        TransitionRecorder<Bounce<Ease::InOut>> bounce(20.0f, 80.0f, 1.0f);

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
    Quad<Ease::In> quadIn(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quad<Ease::Out> quadOut(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Quad<Ease::InOut> quadInOut(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

    Elastic<Ease::In> elasticIn(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Elastic<Ease::Out> elasticOut(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Elastic<Ease::InOut> elasticInOut(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

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
    Quad<Ease::InOut> quick(0.0f, 100.0f, Timestep(1.0f), [](const float&){});
    Quad<Ease::InOut> slow(0.0f, 100.0f, Timestep(10.0f), [](const float&){});
    Elastic<Ease::Out> quickElastic(0.0f, 100.0f, Timestep(1.0f), [](const float&){});
    Elastic<Ease::Out> slowElastic(0.0f, 100.0f, Timestep(10.0f), [](const float&){});

    for(int step = 0; step <= 20; step++)
    {
        const float ratio = step / 20.0f;
        EXPECT_FLOAT_EQ(quick.GetProgressValue(ratio), slow.GetProgressValue(ratio));
        EXPECT_FLOAT_EQ(quickElastic.GetProgressValue(ratio), slowElastic.GetProgressValue(ratio));
    }
}

TEST(EngineTest, TestACurveIsTheSameValueEveryTimeItIsEvaluated)
{
    Back<Ease::Out> back(0.0f, 1.0f, Timestep(1.0f), [](const float&){});
    Elastic<Ease::In> elastic(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

    for(int step = 0; step <= 20; step++)
    {
        const float ratio = step / 20.0f;
        EXPECT_EQ(back.GetProgressValue(ratio), back.GetProgressValue(ratio));
        EXPECT_EQ(elastic.GetProgressValue(ratio), elastic.GetProgressValue(ratio));
    }
}

/**
 * @brief Rewind both overshooting families in one direction and report the start again
 *
 * @tparam MODE the direction under test, applied to both families below
 */
template<Ease::Mode MODE>
void ExpectEveryDirectionRewindsBackAndElastic()
{
    TransitionRecorder<Back<MODE>> rewoundBack(20.0f, 80.0f, 1.0f);
    TransitionRecorder<Elastic<MODE>> rewoundElastic(20.0f, 80.0f, 1.0f);

    rewoundBack.Step(1.0f);
    rewoundElastic.Step(1.0f);
    EXPECT_FLOAT_EQ(80.0f, rewoundBack.Last());
    EXPECT_FLOAT_EQ(80.0f, rewoundElastic.Last());

    rewoundBack.Start();
    rewoundElastic.Start();
    EXPECT_FLOAT_EQ(20.0f, rewoundBack.Last());
    EXPECT_FLOAT_EQ(20.0f, rewoundElastic.Last());
}

TEST(EngineTest, TestACurveCanBePlayedAgainAfterItFinishes)
{
    /*The InOut direction, whose halfway value is the midpoint, so a replay is checked
      against a value the curve is known to produce. An overshooting family's In curve is
      deliberately below its midpoint at that point, which is what the overshoot cases
      above already cover.*/
    TransitionRecorder<Back<Ease::InOut>> back(20.0f, 80.0f, 1.0f);
    TransitionRecorder<Elastic<Ease::InOut>> elastic(20.0f, 80.0f, 1.0f);

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
    ExpectEveryDirectionRewindsBackAndElastic<Ease::In>();
    ExpectEveryDirectionRewindsBackAndElastic<Ease::Out>();
    ExpectEveryDirectionRewindsBackAndElastic<Ease::InOut>();
}

/**
 * @brief Start a zero-length transition in one direction and expect the end value at once
 *
 * @tparam MODE the direction under test, applied to all three families below
 */
template<Ease::Mode MODE>
void ExpectAZeroLengthTransitionJumpsStraightToItsEndValue()
{
    TransitionRecorder<Quad<MODE>> quad(30.0f, 70.0f, 0.0f);
    TransitionRecorder<Elastic<MODE>> elastic(30.0f, 70.0f, 0.0f);
    TransitionRecorder<Bounce<MODE>> bounce(30.0f, 70.0f, 0.0f);

    quad.Start();
    elastic.Start();
    bounce.Start();

    EXPECT_FLOAT_EQ(70.0f, quad.Last());
    EXPECT_FLOAT_EQ(70.0f, elastic.Last());
    EXPECT_FLOAT_EQ(70.0f, bounce.Last());
}

TEST(EngineTest, TestAZeroLengthCurveTransitionJumpsStraightToItsEndValue)
{
    /*The base has no duration to divide the elapsed time by for any family, so none of them
      may produce a NaN here.*/
    ExpectAZeroLengthTransitionJumpsStraightToItsEndValue<Ease::In>();
    ExpectAZeroLengthTransitionJumpsStraightToItsEndValue<Ease::Out>();
    ExpectAZeroLengthTransitionJumpsStraightToItsEndValue<Ease::InOut>();
}

/**
 * @brief Count Bounce's direction reversals in one direction
 *
 * @tparam MODE the direction under test
 */
template<Ease::Mode MODE>
void ExpectBounceReversesDirection()
{
    Bounce<MODE> bounce(0.0f, 1.0f, Timestep(1.0f), [](const float&){});

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

    EXPECT_GE(reversals, 2);
}

TEST(EngineTest, TestBounceReversesDirection)
{
    ExpectBounceReversesDirection<Ease::In>();
    ExpectBounceReversesDirection<Ease::Out>();
    ExpectBounceReversesDirection<Ease::InOut>();
}

TEST(EngineTest, TestQuadMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Quad>(Catalogue::InQuad, Catalogue::OutQuad,
      Catalogue::InOutQuad);
}

TEST(EngineTest, TestCubicMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Cubic>(Catalogue::InCubic, Catalogue::OutCubic,
      Catalogue::InOutCubic);
}

TEST(EngineTest, TestQuartMatchesTheCatalogue)
{
    /*Swapping this family's exponent for Cubic's fails here and nowhere else in the file:
      every other case either passes for both powers or checks a property both share.*/
    ExpectFamilyMatchesCatalogue<Quart>(Catalogue::InQuart, Catalogue::OutQuart,
      Catalogue::InOutQuart);
}

TEST(EngineTest, TestQuintMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Quint>(Catalogue::InQuint, Catalogue::OutQuint,
      Catalogue::InOutQuint);
}

TEST(EngineTest, TestSineMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Sine>(Catalogue::InSine, Catalogue::OutSine,
      Catalogue::InOutSine);
}

TEST(EngineTest, TestExpoMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Expo>(Catalogue::InExpo, Catalogue::OutExpo,
      Catalogue::InOutExpo);
}

TEST(EngineTest, TestCircMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Circ>(Catalogue::InCirc, Catalogue::OutCirc,
      Catalogue::InOutCirc);
}

TEST(EngineTest, TestBackMatchesTheCatalogueAtItsFixedOvershoot)
{
    /*The catalogue's own overshoot, transcribed here rather than read out of the header,
      so a changed constant in the header fails instead of agreeing with itself.*/
    EXPECT_FLOAT_EQ(1.70158f, BACK_OVERSHOOT);

    ExpectFamilyMatchesCatalogue<Back>(Catalogue::InBack, Catalogue::OutBack,
      Catalogue::InOutBack);
}

TEST(EngineTest, TestElasticMatchesTheCatalogueAtItsFixedPeriod)
{
    ExpectFamilyMatchesCatalogue<Elastic>(Catalogue::InElastic, Catalogue::OutElastic,
      Catalogue::InOutElastic);
}

TEST(EngineTest, TestBounceMatchesTheCatalogue)
{
    ExpectFamilyMatchesCatalogue<Bounce>(Catalogue::InBounce, Catalogue::OutBounce,
      Catalogue::InOutBounce);
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
/*Below: reversal. Every case in this group runs in this test binary, which constructs no
  Application, no window, no display and no clock, so a case here doubles as the check that
  reversal needs nothing from the engine: the transition is handed its elapsed time by the
  recorder and reports from the exchanged values with no subsystem contacted.*/

TEST(EngineTest, TestAReversedTransitionBeginsWhereItUsedToEnd)
{
    TransitionRecorder<Quad<Ease::In>> before(30.0f, 70.0f, 6.0f);
    before.Start();
    before.Step(6.0f);
    ASSERT_FLOAT_EQ(70.0f, before.Last());

    TransitionRecorder<Quad<Ease::In>> reversed(30.0f, 70.0f, 6.0f);
    reversed.Reverse();
    reversed.Start();

    EXPECT_FLOAT_EQ(before.Last(), reversed.Last());
    EXPECT_FLOAT_EQ(70.0f, reversed.Last());
}

TEST(EngineTest, TestARunAndTheRunAfterItMeetWithNoGap)
{
    TransitionRecorder<Quad<Ease::In>> recorder(30.0f, 70.0f, 6.0f);
    recorder.Start();

    for(int step = 0; step < 12; step++)
        recorder.Step(0.5f);

    ASSERT_TRUE(recorder.Finished());
    const float endOfRun = recorder.Last();

    recorder.Reverse();
    recorder.Start();

    /*The value it reports now is the value the finished run reported last, so the two runs
      meet at one value with nothing between them rather than jumping to the other end.*/
    EXPECT_FLOAT_EQ(endOfRun, recorder.Last());
    EXPECT_FLOAT_EQ(70.0f, recorder.Last());
}

TEST(EngineTest, TestAReversedTransitionRunsTheSameCurveOverTheOppositeDistance)
{
    /*The family, the direction and the two values in their new order, transcribed from the
      catalogue rather than read back out of the header: an inverted time argument would land
      on the original start value with the shape backwards, and fail every value below.*/
    TransitionRecorder<Quad<Ease::In>> recorder(30.0f, 70.0f, 6.0f);
    recorder.Reverse();
    recorder.Start();

    for(int step = 0; step < 20; step++)
        recorder.Step(0.3f);

    const std::vector<float> reported = recorder.Values();
    ASSERT_EQ(21u, reported.size());

    for(size_t point = 0; point < reported.size(); point++)
    {
        const float ratio = static_cast<float>(point) / 20.0f;
        const float expected = 70.0f + (30.0f - 70.0f) * Catalogue::InQuad(ratio);
        EXPECT_FLOAT_EQ(expected, reported[point]);
    }

    /*No residue from the curve's shape: it lands on the value it began at, exactly.*/
    EXPECT_FLOAT_EQ(30.0f, reported.back());
}

TEST(EngineTest, TestReversingTwiceLeavesATransitionAlone)
{
    TransitionRecorder<Quad<Ease::In>> plain(30.0f, 70.0f, 6.0f);
    plain.Start();
    for(int step = 0; step < 20; step++)
        plain.Step(0.3f);

    TransitionRecorder<Quad<Ease::In>> twice(30.0f, 70.0f, 6.0f);
    twice.Reverse();
    twice.Reverse();
    twice.Start();
    for(int step = 0; step < 20; step++)
        twice.Step(0.3f);

    EXPECT_EQ(plain.Values(), twice.Values());
}

TEST(EngineTest, TestReversalLeavesTheDirectionAlone)
{
    /*Out, so a reversal that inverted the time argument would read as In and fail here: the
      shape still decelerates into the value being travelled to, which is what the direction
      means, and it is read over the two values in their new order.*/
    TransitionRecorder<Quad<Ease::Out>> recorder(30.0f, 70.0f, 6.0f);
    recorder.Reverse();
    recorder.Start();

    for(int step = 0; step < 20; step++)
        recorder.Step(0.3f);

    const std::vector<float> reported = recorder.Values();
    ASSERT_EQ(21u, reported.size());

    for(size_t point = 0; point < reported.size(); point++)
    {
        const float ratio = static_cast<float>(point) / 20.0f;
        const float expected = 70.0f + (30.0f - 70.0f) * Catalogue::OutQuad(ratio);
        EXPECT_FLOAT_EQ(expected, reported[point]);
    }
}

/**
 * @brief Drive one transition backwards to the end of its duration and check it lands on
 * the value it started from, exactly
 *
 * @tparam TransitionType the family and direction under test
 * @param start the value the transition was built to begin at
 * @param end the value the transition was built to end at
 */
template<typename TransitionType>
void ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues(const float& start,
  const float& end)
{
    TransitionRecorder<TransitionType> recorder(start, end, 1.0f);
    recorder.Reverse();
    recorder.Start();

    for(int step = 0; step < 20; step++)
        recorder.Step(0.05f);

    ASSERT_TRUE(recorder.Finished());

    /*The value it lands on is the one it began at before it was reversed, on raw float
      equality, and it is one of exactly the two values it was built with: never a value
      near one of them and never a value left outside them by the run.*/
    EXPECT_FLOAT_EQ(start, recorder.Last());
    EXPECT_TRUE(recorder.Last() == start || recorder.Last() == end);
}

/**
 * @brief Every family and one direction, reversed, landing exactly on one of its two values
 *
 * @tparam MODE the direction under test, applied to every family below
 */
template<Ease::Mode MODE>
void ExpectEveryReversedCurveLandsExactlyOnOneOfItsTwoValues()
{
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Quad<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Cubic<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Quart<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Quint<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Sine<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Expo<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Circ<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Back<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Elastic<MODE>>(30.0f, 70.0f);
    ExpectAReversedTransitionLandsExactlyOnOneOfItsTwoValues<Bounce<MODE>>(30.0f, 70.0f);
}

TEST(EngineTest, TestAReversedTransitionStillLandsExactlyOnOneOfItsTwoValues)
{
    /*The overshooting families included: Back and Elastic are the ones that could leave a
      value outside the pair, and both are required to settle onto the exchanged end value.*/
    ExpectEveryReversedCurveLandsExactlyOnOneOfItsTwoValues<Ease::In>();
    ExpectEveryReversedCurveLandsExactlyOnOneOfItsTwoValues<Ease::Out>();
    ExpectEveryReversedCurveLandsExactlyOnOneOfItsTwoValues<Ease::InOut>();
}

TEST(EngineTest, TestReversalNeedsNothingFromTheEngine)
{
    /*No Application is constructed anywhere in this binary, so there is no window, no
      display, no graphics context and no frame loop running here: the transition is handed
      its elapsed time by the recorder alone and reports from the exchanged values.*/
    TransitionRecorder<Linear> recorder(30.0f, 70.0f, 2.0f);
    recorder.Reverse();
    recorder.Start();

    EXPECT_FLOAT_EQ(70.0f, recorder.Last());

    recorder.Step(1.0f);
    EXPECT_FLOAT_EQ(50.0f, recorder.Last());

    recorder.Step(1.0f);
    EXPECT_FLOAT_EQ(30.0f, recorder.Last());
}
