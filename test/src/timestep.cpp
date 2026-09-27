#include <cmath>
#include <gtest/gtest.h>
#include "SekaiEngine/Timer.h"

TEST(EngineTest, TestTimestepOperatorEqualTrue)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left == right);
}

TEST(EngineTest, TestTimestepOperatorEqualFalse)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_FALSE(left == right);
}

TEST(EngineTest, TestTimestepOperatorNotEqualTrue)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_TRUE(left != right);
}

TEST(EngineTest, TestTimestepOperatorNotEqualFalse)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left != right);
}

TEST(EngineTest, TestTimestepOperatorLessThanTrue)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_TRUE(left < right);
}

TEST(EngineTest, TestTimestepOperatorLessThanByEqual)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left < right);
}

TEST(EngineTest, TestTimestepOperatorLessThanByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left < right);
}

TEST(EngineTest, TestTimestepOperatorLessThanOrEqualToByLessThan)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_TRUE(left <= right);
}

TEST(EngineTest, TestTimestepOperatorLessThanOrEqualToByEqualTo)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left <= right);
}

TEST(EngineTest, TestTimestepOperatorLessThanOrEqualToByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left <= right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left > right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanByEqual)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left > right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanByLessThan)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_FALSE(left > right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanOrEqualByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left >= right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanOrEqualByEqualTo)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left >= right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanOrEqualByLessThan)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_FALSE(left >= right);
}

/*Below: the frame rate Timer reports. These need no window and no display, because the
  counter is fed by Timer::update(), which needs nothing but a clock.*/

TEST(EngineTest, TestFPSIsZeroBeforeTheFirstWindowCloses)
{
    SekaiEngine::Timer timer;

    EXPECT_FLOAT_EQ(timer.FPS(), 0.0f);

    /*Repeated updates inside the first window must not move it, and must not divide by the
      near-zero duration of the first frame either.*/
    for(int i = 0; i < 8; i++)
    {
        timer.update();
        EXPECT_FLOAT_EQ(timer.FPS(), 0.0f);
    }
}

TEST(EngineTest, TestFPSIsAPositiveFiniteRateOnceAWindowCloses)
{
    SekaiEngine::Timer timer;

    /*Bounded so a machine too slow to close a window fails the test rather than hanging
      it. The rate itself is whatever this machine manages, so only its shape is checked.*/
    float fps = 0.0f;
    int frames = 0;
    while(fps == 0.0f && frames < 100000000)
    {
        timer.update();
        fps = timer.FPS();
        frames++;
    }

    ASSERT_GT(frames, 0);
    ASSERT_LT(frames, 100000000);
    EXPECT_TRUE(std::isfinite(fps));
    EXPECT_GT(fps, 0.0f);
    /*The ceiling is the clock's own rate, not a game's: a tight loop of update() calls
      manages millions of "frames" a second, so this only has to be generous enough not to
      fail on a fast machine and tight enough to catch a value in the wrong unit.*/
    EXPECT_LT(fps, 1000000000.0f);
}

TEST(EngineTest, TestFPSHoldsSteadyWithinAWindow)
{
    SekaiEngine::Timer timer;
    float closed = 0.0f;
    int frames = 0;

    /*Read every frame up to and past the frame that closes the window: the value must be
      a whole rate, not a per-frame reciprocal, so it does not alternate between two
      readings from one frame to the next.*/
    while(closed == 0.0f && frames < 100000000)
    {
        timer.update();
        frames++;
        float fps = timer.FPS();
        if(fps > 0.0f)
        {
            closed = fps;
            EXPECT_FLOAT_EQ(timer.FPS(), closed);
        }
    }

    ASSERT_GT(closed, 0.0f);
    /*One more frame is nowhere near a whole window on any machine that can run a test, and
      it must not move the value: that is the difference between a rate over an interval
      and a per-frame reciprocal.*/
    timer.update();
    EXPECT_FLOAT_EQ(timer.FPS(), closed);
    EXPECT_FLOAT_EQ(timer.FPS(), closed);
}
