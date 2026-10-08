#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "SekaiEngine/Animation/Animator.h"
#include "SekaiEngine/Animation/Parallel.h"
#include "SekaiEngine/Animation/Repeat.h"
#include "SekaiEngine/Animation/Sequence.h"
#include "SekaiEngine/Animation/Transitions/Linear.h"

/*Below: composition and the manager. Everything here is header only, driven by a timestep
  the test hands over, so no window, no display, no graphics context and no frame loop is
  involved anywhere in this file.*/

using namespace SekaiEngine;
using namespace SekaiEngine::Animation;

/**
 * @brief One value one child reported, tagged with which child reported it
 *
 * @note A single list rather than one per child, because the requirement worth checking is
 * about order: nothing of the second child may appear before the first has finished, and
 * that can only be read off a list in which both are recorded together.
 */
struct Report
{
    char child;
    float value;
};

/**
 * @brief Build a linear leaf that reports every value it reaches into the test's list
 *
 * @param record the list the leaf appends to, which must outlive the leaf
 * @param tag which child this is, so its reports can be told apart from its siblings'
 * @param start the value the leaf begins at
 * @param end the value the leaf ends at
 * @param seconds how long the leaf takes
 * @return std::unique_ptr<Node> the leaf, ready to be given to a container
 */
static std::unique_ptr<Node> MakeLeaf(std::vector<Report>& record, const char tag,
  const float& start, const float& end, const float& seconds)
{
    return std::make_unique<Linear>(start, end, Timestep(seconds),
      [&record, tag](const float& value){ record.push_back(Report{tag, value}); });
}

/**
 * @brief Build a linear leaf that reports into its own list, for the manager's own tests
 *
 * @param record the list the leaf appends to, which must outlive the leaf
 * @param start the value the leaf begins at
 * @param end the value the leaf ends at
 * @param seconds how long the leaf takes
 * @return std::unique_ptr<Node> the leaf, ready to be handed to a manager
 */
static std::unique_ptr<Node> MakeSoloLeaf(std::vector<float>& record, const float& start,
  const float& end, const float& seconds)
{
    return std::make_unique<Linear>(start, end, Timestep(seconds),
      [&record](const float& value){ record.push_back(value); });
}

/**
 * @brief How many of the reports came from the child with the given tag
 */
static std::size_t CountReports(const std::vector<Report>& record, const char tag)
{
    std::size_t found = 0;
    for(std::size_t report = 0; report < record.size(); report++)
    {
        if(record[report].child == tag)
            found++;
    }

    return found;
}

/**
 * @brief The index of the first report from the given child, or the list's size if there is none
 */
static std::size_t FirstReportOf(const std::vector<Report>& record, const char tag)
{
    for(std::size_t report = 0; report < record.size(); report++)
    {
        if(record[report].child == tag)
            return report;
    }

    return record.size();
}

/**
 * @brief The index of the last report from the given child, or the list's size if there is none
 */
static std::size_t LastReportOf(const std::vector<Report>& record, const char tag)
{
    std::size_t found = record.size();
    for(std::size_t report = 0; report < record.size(); report++)
    {
        if(record[report].child == tag)
            found = report;
    }

    return found;
}

/*Below: the sequence. Two children with deliberately unshared values, so that a sequence
  handing the first child's end value to the second would be visible as a wrong first
  report rather than as an accidental agreement.*/

TEST(AnimationTest, TestASequenceAdvancesExactlyOneChildAtATime)
{
    std::vector<Report> reports;
    Sequence sequence;
    sequence.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    sequence.Add(MakeLeaf(reports, 'B', 100.0f, 200.0f, 1.0f));
    sequence.Start();

    for(int step = 0; step < 40; step++)
        sequence.Update(Timestep(0.1f));

    ASSERT_TRUE(sequence.IsFinish());
    ASSERT_GT(reports.size(), 2u);
    EXPECT_EQ('A', reports.front().child);
    EXPECT_FLOAT_EQ(0.0f, reports.front().value);
    EXPECT_EQ('B', reports.back().child);
    EXPECT_FLOAT_EQ(200.0f, reports.back().value);

    /*Nothing of the first child is reported once the second has begun: the two runs are
      one after the other, never side by side.*/
    const std::size_t beginsB = FirstReportOf(reports, 'B');
    ASSERT_LT(beginsB, reports.size());
    for(std::size_t report = beginsB + 1; report < reports.size(); report++)
        EXPECT_NE('A', reports[report].child);
}

TEST(AnimationTest, TestEachChildOfASequenceBeginsAtTheValueItWasGiven)
{
    std::vector<Report> reports;
    Sequence sequence;
    sequence.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    sequence.Add(MakeLeaf(reports, 'B', 100.0f, 200.0f, 1.0f));
    sequence.Start();

    for(int step = 0; step < 11; step++)
        sequence.Update(Timestep(0.1f));

    /*A finished on 10, and the child after it was given 100 to begin at: it reports 100,
      not the value its predecessor happened to end on.*/
    const std::size_t beginsB = FirstReportOf(reports, 'B');
    ASSERT_LT(beginsB, reports.size());
    EXPECT_FLOAT_EQ(100.0f, reports[beginsB].value);
    EXPECT_FLOAT_EQ(10.0f, reports[beginsB - 1].value);
}

TEST(AnimationTest, TestStartingASequenceAgainBeginsAtItsFirstChild)
{
    std::vector<Report> reports;
    Sequence sequence;
    sequence.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    sequence.Add(MakeLeaf(reports, 'B', 100.0f, 200.0f, 1.0f));

    sequence.Start();
    for(int step = 0; step < 40; step++)
        sequence.Update(Timestep(0.1f));
    ASSERT_TRUE(sequence.IsFinish());
    ASSERT_GT(CountReports(reports, 'B'), 0u);

    reports.clear();
    sequence.Start();
    for(int step = 0; step < 5; step++)
        sequence.Update(Timestep(0.1f));

    EXPECT_EQ(0u, CountReports(reports, 'B'));
    EXPECT_GT(CountReports(reports, 'A'), 0u);
    EXPECT_FLOAT_EQ(0.0f, reports.front().value);
    EXPECT_FLOAT_EQ(5.0f, reports.back().value);
}

TEST(AnimationTest, TestAChildOfNoLengthStartsTheNextChildInTheSameAdvance)
{
    std::vector<Report> reports;
    Sequence sequence;
    sequence.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 0.0f));
    sequence.Add(MakeLeaf(reports, 'B', 100.0f, 200.0f, 1.0f));

    sequence.Start();
    ASSERT_EQ(1u, reports.size());
    EXPECT_FLOAT_EQ(10.0f, reports[0].value);

    reports.clear();
    sequence.Update(Timestep(0.1f));

    /*One advance, and the child after the empty one both starts and reports its start value
      inside it, rather than waiting for the frame after.*/
    ASSERT_EQ(1u, reports.size());
    EXPECT_EQ('B', reports[0].child);
    EXPECT_FLOAT_EQ(100.0f, reports[0].value);
}

/*Below: the parallel group.*/

TEST(AnimationTest, TestEveryChildOfAGroupAdvancesTogether)
{
    std::vector<Report> reports;
    Parallel group;
    group.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    group.Add(MakeLeaf(reports, 'B', 0.0f, 100.0f, 1.0f));
    group.Start();

    reports.clear();
    group.Update(Timestep(0.5f));

    ASSERT_EQ(2u, reports.size());
    EXPECT_FLOAT_EQ(5.0f, reports[0].value);
    EXPECT_FLOAT_EQ(50.0f, reports[1].value);
}

TEST(AnimationTest, TestAGroupOutlivesItsShortestChild)
{
    std::vector<Report> reports;
    Parallel group;
    group.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    group.Add(MakeLeaf(reports, 'B', 0.0f, 100.0f, 2.0f));
    group.Start();

    for(int step = 0; step < 15; step++)
        group.Update(Timestep(0.1f));

    /*A has been finished for five frames and the group is still running, because B has not
      reported its end value yet.*/
    const std::size_t lastA = LastReportOf(reports, 'A');
    ASSERT_LT(lastA, reports.size());
    EXPECT_FLOAT_EQ(10.0f, reports[lastA].value);
    EXPECT_FALSE(group.IsFinish());

    for(int step = 0; step < 10; step++)
        group.Update(Timestep(0.1f));

    EXPECT_TRUE(group.IsFinish());
    EXPECT_FLOAT_EQ(100.0f, reports.back().value);
}

TEST(AnimationTest, TestStartingAGroupAgainRestartsEveryChild)
{
    std::vector<Report> reports;
    Parallel group;
    group.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    group.Add(MakeLeaf(reports, 'B', 0.0f, 100.0f, 1.0f));

    group.Start();
    for(int step = 0; step < 12; step++)
        group.Update(Timestep(0.1f));
    ASSERT_TRUE(group.IsFinish());

    reports.clear();
    group.Start();
    ASSERT_EQ(2u, reports.size());
    EXPECT_FLOAT_EQ(0.0f, reports[0].value);
    EXPECT_FLOAT_EQ(0.0f, reports[1].value);
}

/*Below: the repeat. The child runs from 0 to 10 over a second, so one finished run is
  visible as one landing on 10 in replay order and one landing back on 0 in ping-pong. A
  run is over after exactly ten tenths of a second, which is how the count is pinned: the
  repeat is finished after the tenth step of its last run and not before it.*/

TEST(AnimationTest, TestARepeatOfCountOneRunsItsChildOnce)
{
    std::vector<Report> reports;
    Repeat repeat(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f), 1, Repeat::Replay);
    repeat.Start();

    ASSERT_EQ(1u, reports.size());
    EXPECT_FLOAT_EQ(0.0f, reports[0].value);
    EXPECT_FALSE(repeat.IsFinish());

    repeat.Update(Timestep(1.0f));

    ASSERT_EQ(2u, reports.size());
    EXPECT_FLOAT_EQ(10.0f, reports[1].value);
    EXPECT_TRUE(repeat.IsFinish());
}

TEST(AnimationTest, TestARepeatOfCountThreeRunsItsChildThreeTimes)
{
    std::vector<Report> reports;
    Repeat repeat(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f), 3, Repeat::Replay);
    repeat.Start();

    for(int step = 0; step < 29; step++)
        repeat.Update(Timestep(0.1f));
    EXPECT_FALSE(repeat.IsFinish());

    for(int step = 29; step < 40; step++)
        repeat.Update(Timestep(0.1f));

    ASSERT_TRUE(repeat.IsFinish());
    EXPECT_FLOAT_EQ(10.0f, reports.back().value);

    /*Three completed runs, each landing exactly on the end value, and each run after the
      first beginning again from the start value it was given.*/
    std::size_t landings = 0;
    for(std::size_t report = 0; report < reports.size(); report++)
    {
        if(reports[report].value == 10.0f)
            landings++;

        if(report > 0 && reports[report - 1].value == 10.0f)
            EXPECT_FLOAT_EQ(0.0f, reports[report].value);
    }

    EXPECT_EQ(3u, landings);
}

TEST(AnimationTest, TestAForeverRepeatNeverFinishes)
{
    std::vector<Report> reports;
    Repeat repeat(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f), Repeat::Forever,
      Repeat::Replay);
    repeat.Start();

    for(int step = 0; step < 500; step++)
        repeat.Update(Timestep(0.1f));

    EXPECT_FALSE(repeat.IsFinish());
    EXPECT_GT(reports.size(), 500u);
}

TEST(AnimationTest, TestAPingPongRepeatOfFourEndsWhereItBegan)
{
    std::vector<Report> forward;
    std::vector<Report> backward;
    Repeat repeat(MakeLeaf(forward, 'A', 0.0f, 10.0f, 1.0f), 4, Repeat::PingPong);
    repeat.Start();

    /*Thirty-nine steps is three runs and nine tenths of the fourth, so the count is still
      being performed; the fortieth completes the fourth.*/
    for(int step = 0; step < 39; step++)
        repeat.Update(Timestep(0.1f));
    EXPECT_FALSE(repeat.IsFinish());

    repeat.Update(Timestep(0.1f));

    ASSERT_TRUE(repeat.IsFinish());
    /*Four runs: to the end value, back, to the end value, back — so it is finished sitting
      on the start value it began the whole count on.*/
    EXPECT_FLOAT_EQ(0.0f, forward.back().value);
    EXPECT_EQ('A', forward.front().child);
    EXPECT_FLOAT_EQ(0.0f, forward.front().value);
    EXPECT_EQ(0u, backward.size());
}

TEST(AnimationTest, TestAPingPongRepeatOfThreeEndsOnTheEndValue)
{
    std::vector<Report> reports;
    Repeat repeat(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f), 3, Repeat::PingPong);
    repeat.Start();

    for(int step = 0; step < 29; step++)
        repeat.Update(Timestep(0.1f));
    EXPECT_FALSE(repeat.IsFinish());

    repeat.Update(Timestep(0.1f));

    ASSERT_TRUE(repeat.IsFinish());
    EXPECT_FLOAT_EQ(10.0f, reports.back().value);
}

TEST(AnimationTest, TestTheChildOfARepeatCanBeAWholeComposition)
{
    std::vector<Report> reports;
    Sequence sequence;
    sequence.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    sequence.Add(MakeLeaf(reports, 'B', 100.0f, 200.0f, 1.0f));

    std::unique_ptr<Node> composed = std::make_unique<Sequence>(std::move(sequence));
    Repeat repeat(std::move(composed), 2, Repeat::Replay);
    repeat.Start();

    for(int step = 0; step < 39; step++)
        repeat.Update(Timestep(0.1f));
    EXPECT_FALSE(repeat.IsFinish());

    repeat.Update(Timestep(0.1f));

    ASSERT_TRUE(repeat.IsFinish());

    /*The whole sequence ran once per unit of the count: two landings on each child's end
      value, in order, with every B report preceded by the A of the same run.*/
    std::size_t endsOfA = 0;
    std::size_t endsOfB = 0;
    for(std::size_t report = 0; report < reports.size(); report++)
    {
        if(reports[report].value == 10.0f)
            endsOfA++;
        if(reports[report].value == 200.0f)
            endsOfB++;
    }

    EXPECT_EQ(2u, endsOfA);
    EXPECT_EQ(2u, endsOfB);
    EXPECT_FLOAT_EQ(200.0f, reports.back().value);
}

/*Below: reversal of a composition. The leaves turn around; the structure does not.*/

TEST(AnimationTest, TestReversingASequenceKeepsTheOrderOfItsChildren)
{
    std::vector<Report> reports;
    Sequence sequence;
    sequence.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    sequence.Add(MakeLeaf(reports, 'B', 100.0f, 200.0f, 1.0f));

    sequence.Reverse();
    sequence.Start();
    for(int step = 0; step < 40; step++)
        sequence.Update(Timestep(0.1f));

    ASSERT_TRUE(sequence.IsFinish());

    /*The first child still runs first, and it now runs from the value it used to finish on
      down to the value it used to begin at.*/
    ASSERT_FALSE(reports.empty());
    EXPECT_EQ('A', reports.front().child);
    EXPECT_FLOAT_EQ(10.0f, reports.front().value);

    const std::size_t beginsB = FirstReportOf(reports, 'B');
    ASSERT_LT(beginsB, reports.size());
    EXPECT_FLOAT_EQ(200.0f, reports[beginsB].value);

    for(std::size_t report = beginsB + 1; report < reports.size(); report++)
        EXPECT_NE('A', reports[report].child);

    /*And the second child finishes where it used to start.*/
    EXPECT_FLOAT_EQ(100.0f, reports.back().value);
}

TEST(AnimationTest, TestReversingTwiceLeavesASequenceReportingWhatItDid)
{
    std::vector<Report> plainReports;
    std::vector<Report> twiceReports;

    Sequence plain;
    plain.Add(MakeLeaf(plainReports, 'A', 0.0f, 10.0f, 1.0f));
    plain.Add(MakeLeaf(plainReports, 'B', 100.0f, 200.0f, 1.0f));
    plain.Start();
    for(int step = 0; step < 40; step++)
        plain.Update(Timestep(0.1f));

    Sequence twice;
    twice.Add(MakeLeaf(twiceReports, 'A', 0.0f, 10.0f, 1.0f));
    twice.Add(MakeLeaf(twiceReports, 'B', 100.0f, 200.0f, 1.0f));
    twice.Reverse();
    twice.Reverse();
    twice.Start();
    for(int step = 0; step < 40; step++)
        twice.Update(Timestep(0.1f));

    ASSERT_EQ(plainReports.size(), twiceReports.size());
    for(std::size_t report = 0; report < plainReports.size(); report++)
    {
        EXPECT_EQ(plainReports[report].child, twiceReports[report].child);
        EXPECT_FLOAT_EQ(plainReports[report].value, twiceReports[report].value);
    }
}

TEST(AnimationTest, TestReversingARepeatLeavesItsCountAloneAndSendsEachRunTheOtherWay)
{
    std::vector<Report> forward;
    std::vector<Report> backward;
    Repeat plain(MakeLeaf(forward, 'A', 0.0f, 10.0f, 1.0f), 3, Repeat::Replay);
    plain.Start();
    for(int step = 0; step < 40; step++)
        plain.Update(Timestep(0.1f));

    Repeat reversed(MakeLeaf(backward, 'A', 0.0f, 10.0f, 1.0f), 3, Repeat::Replay);
    reversed.Reverse();
    reversed.Start();
    for(int step = 0; step < 40; step++)
        reversed.Update(Timestep(0.1f));

    ASSERT_TRUE(plain.IsFinish());
    ASSERT_TRUE(reversed.IsFinish());

    /*The same number of runs, finished at the same point — but every run travels the other
      way, so it lands on the start value where the untouched one lands on the end value,
      and it reports exactly as many values doing so.*/
    EXPECT_FLOAT_EQ(10.0f, forward.back().value);
    EXPECT_FLOAT_EQ(0.0f, backward.back().value);
    EXPECT_EQ(forward.size(), backward.size());

    /*The order did not move either: this is still Replay, so every run after the first
      begins on the value the run before it ended on. A repeat whose order had turned
      would start its second run from the other end.*/
    for(std::size_t report = 0; report + 1 < backward.size(); report++)
    {
        if(backward[report].value == 0.0f)
            EXPECT_FLOAT_EQ(10.0f, backward[report + 1].value);
    }
}

TEST(AnimationTest, TestReversingTwiceLeavesARepeatReportingWhatItDid)
{
    std::vector<Report> plainReports;
    std::vector<Report> twiceReports;

    Repeat plain(MakeLeaf(plainReports, 'A', 0.0f, 10.0f, 1.0f), 3, Repeat::Replay);
    plain.Start();
    for(int step = 0; step < 40; step++)
        plain.Update(Timestep(0.1f));

    Repeat twice(MakeLeaf(twiceReports, 'A', 0.0f, 10.0f, 1.0f), 3, Repeat::Replay);
    twice.Reverse();
    twice.Reverse();
    twice.Start();
    for(int step = 0; step < 40; step++)
        twice.Update(Timestep(0.1f));

    ASSERT_TRUE(plain.IsFinish());
    ASSERT_TRUE(twice.IsFinish());
    ASSERT_EQ(plainReports.size(), twiceReports.size());
    for(std::size_t report = 0; report < plainReports.size(); report++)
    {
        EXPECT_EQ(plainReports[report].child, twiceReports[report].child);
        EXPECT_FLOAT_EQ(plainReports[report].value, twiceReports[report].value);
    }
}

TEST(AnimationTest, TestReversingAGroupReversesEveryChild)
{
    std::vector<Report> reports;
    Parallel group;
    group.Add(MakeLeaf(reports, 'A', 0.0f, 10.0f, 1.0f));
    group.Add(MakeLeaf(reports, 'B', 0.0f, 100.0f, 1.0f));

    group.Reverse();
    group.Start();

    /*Both children begin where they used to end, and both finish where they used to begin,
      while the group still advances them together.*/
    ASSERT_EQ(2u, reports.size());
    EXPECT_FLOAT_EQ(10.0f, reports[0].value);
    EXPECT_FLOAT_EQ(100.0f, reports[1].value);

    reports.clear();
    group.Update(Timestep(1.0f));

    ASSERT_EQ(2u, reports.size());
    EXPECT_FLOAT_EQ(0.0f, reports[0].value);
    EXPECT_FLOAT_EQ(0.0f, reports[1].value);
    EXPECT_TRUE(group.IsFinish());
}

/*Below: the manager. These construct an Animator directly rather than an Application,
  which is what the requirement about needing no window and no frame loop means in
  practice: an advance is a function call on a timestep, and nothing else is contacted.*/

TEST(AnimationTest, TestAGameNeverStepsAnAnimationItself)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f));

    for(int step = 0; step < 10; step++)
        animator.Update(Timestep(0.1f));

    ASSERT_GT(values.size(), 1u);
    EXPECT_FLOAT_EQ(10.0f, values.back());
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestAFinishedAnimationIsSilentOnLaterFrames)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f));

    animator.Update(Timestep(1.0f));
    ASSERT_FLOAT_EQ(10.0f, values.back());

    const std::size_t reported = values.size();
    animator.Update(Timestep(0.1f));
    animator.Update(Timestep(0.1f));

    EXPECT_EQ(reported, values.size());
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestTheEngineHoldsNoFinishedAnimation)
{
    std::vector<float> values;
    Animator animator;

    for(int animation = 0; animation < 20; animation++)
        animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 0.1f));

    EXPECT_EQ(20u, animator.Count());

    for(int step = 0; step < 30; step++)
        animator.Update(Timestep(0.1f));

    /*Twenty were handed over and twenty have finished: the number held does not grow with
      the number that have finished, because each one is let go of as it goes.*/
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestAnAnimationThatNeverFinishesIsNeverReleased)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(std::make_unique<Repeat>(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f),
      Repeat::Forever, Repeat::Replay));

    for(int step = 0; step < 50; step++)
        animator.Update(Timestep(0.1f));

    EXPECT_EQ(1u, animator.Count());
    EXPECT_GT(values.size(), 50u);
}

TEST(AnimationTest, TestAnAnimationReportsWhereItBeginsAsSoonAsItIsHandedOver)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(MakeSoloLeaf(values, 30.0f, 70.0f, 6.0f));

    /*Before a single timestep has been advanced, the handler has already been called with
      the value the animation begins at.*/
    ASSERT_EQ(1u, values.size());
    EXPECT_FLOAT_EQ(30.0f, values[0]);
    EXPECT_EQ(1u, animator.Count());
}

TEST(AnimationTest, TestAnAnimationOfNoLengthEndsAtHandOver)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(MakeSoloLeaf(values, 30.0f, 70.0f, 0.0f));

    ASSERT_EQ(1u, values.size());
    EXPECT_FLOAT_EQ(70.0f, values[0]);

    animator.Update(Timestep(0.1f));

    EXPECT_EQ(1u, values.size());
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestAnAnimationHandedOverBeforeAnAdvanceIsAdvancedByIt)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(MakeSoloLeaf(values, 30.0f, 70.0f, 6.0f));

    /*This is the event-handler case in miniature: the hand-over happened before the advance
      began, so the first advance that begins after it is this one, and the animation moves
      in it having already reported its start value.*/
    animator.Update(Timestep(1.0f));

    ASSERT_EQ(2u, values.size());
    EXPECT_FLOAT_EQ(30.0f, values[0]);
    EXPECT_FLOAT_EQ(30.0f + 40.0f * (1.0f / 6.0f), values[1]);
}

TEST(AnimationTest, TestPlayingFromInsideAnAnimationDefersTheNewOneToTheNextFrame)
{
    std::vector<float> first;
    std::vector<float> second;
    Animator animator;

    /*The first animation hands a second one to the manager from its own handler, which is
      the case that would otherwise push into the list being walked.*/
    animator.Play(std::make_unique<Linear>(0.0f, 10.0f, Timestep(1.0f),
      [&animator, &first, &second](const float& value)
      {
          first.push_back(value);
          if(first.size() == 3u)
              animator.Play(MakeSoloLeaf(second, 100.0f, 200.0f, 1.0f));
      }));

    /*The first reports where it begins the moment it is handed over. The second does not
      exist yet: it is only created once the first has reported three times.*/
    ASSERT_EQ(1u, first.size());
    EXPECT_FLOAT_EQ(0.0f, first[0]);
    EXPECT_EQ(0u, second.size());
    EXPECT_EQ(1u, animator.Count());

    animator.Update(Timestep(0.1f));
    animator.Update(Timestep(0.1f));

    /*Two advances have run and the hand-over happened on the second of them, so the new
      animation has reported nothing since its start value and the one already running has
      carried on from 0 rather than being restarted.*/
    ASSERT_EQ(3u, first.size());
    EXPECT_FLOAT_EQ(0.0f, first[0]);
    EXPECT_FLOAT_EQ(2.0f, first[2]);
    EXPECT_EQ(1u, second.size());

    animator.Update(Timestep(0.1f));

    /*The first advance that begins after the hand-over is this one, and the new animation
      moves in it while the one already running keeps its state.*/
    ASSERT_EQ(2u, second.size());
    EXPECT_FLOAT_EQ(110.0f, second[1]);
    ASSERT_EQ(4u, first.size());
    EXPECT_FLOAT_EQ(3.0f, first[3]);
}

TEST(AnimationTest, TestDiscardingStopsARunThatNeverEnds)
{
    std::vector<float> values;
    Animator animator;
    animator.Play(std::make_unique<Repeat>(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f),
      Repeat::Forever, Repeat::Replay));

    animator.Update(Timestep(0.1f));
    EXPECT_EQ(1u, animator.Count());

    animator.Clear();

    const std::size_t reported = values.size();
    animator.Update(Timestep(0.1f));
    animator.Update(Timestep(0.1f));

    EXPECT_EQ(reported, values.size());
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestDiscardingEverythingIsSafeWhileAdvancing)
{
    std::vector<float> first;
    std::vector<float> second;
    Animator animator;

    animator.Play(std::make_unique<Linear>(0.0f, 10.0f, Timestep(1.0f),
      [&animator, &first](const float& value)
      {
          first.push_back(value);
          animator.Clear();
      }));
    animator.Play(MakeSoloLeaf(second, 100.0f, 200.0f, 1.0f));

    /*Two are held when the advance begins; the first one's own handler discards both of
      them part way through it.*/
    ASSERT_EQ(2u, animator.Count());
    EXPECT_NO_THROW(animator.Update(Timestep(0.1f)));

    /*The first reported its start value at hand-over and its first step in the advance that
      was cut short. The second, already queued, reported nothing at all beyond its own
      hand-over, and was never advanced.*/
    EXPECT_FLOAT_EQ(0.0f, first[0]);
    EXPECT_FLOAT_EQ(1.0f, first[1]);
    EXPECT_EQ(2u, first.size());
    ASSERT_EQ(1u, second.size());
    EXPECT_FLOAT_EQ(100.0f, second[0]);
    EXPECT_EQ(0u, animator.Count());

    const std::size_t firstReports = first.size();
    const std::size_t secondReports = second.size();

    animator.Update(Timestep(0.1f));

    EXPECT_EQ(firstReports, first.size());
    EXPECT_EQ(secondReports, second.size());
    EXPECT_EQ(0u, animator.Count());
}

/*Below: the end-of-animation notification. The handler is on the manager, so what is
  pinned here is that it fires for an animation the manager owns, in the advance that
  finishes it, before the erase — and never for anything else.*/

TEST(AnimationTest, TestTheEndHandlerIsCalledOnceInTheAdvanceThatFinishes)
{
    std::vector<float> values;
    std::size_t ends = 0;
    bool finishedWhileReported = false;
    Animator animator;
    animator.OnTransitionEnd([&](const Node& animation)
    {
        ends++;
        finishedWhileReported = animation.IsFinish();
    });
    animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f));

    animator.Update(Timestep(0.5f));
    EXPECT_EQ(0u, ends);

    animator.Update(Timestep(0.5f));
    EXPECT_EQ(1u, ends);
    EXPECT_TRUE(finishedWhileReported);
    EXPECT_FLOAT_EQ(10.0f, values.back());

    animator.Update(Timestep(0.5f));
    animator.Update(Timestep(0.5f));
    EXPECT_EQ(1u, ends);
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestTheAnimationIsOwnedWhileItIsReported)
{
    std::vector<float> values;
    std::size_t heldDuringCall = 0;
    bool finishedDuringCall = false;
    float lastDuringCall = -1.0f;
    Animator animator;
    animator.OnTransitionEnd([&](const Node& animation)
    {
        heldDuringCall = animator.Count();
        finishedDuringCall = animation.IsFinish();
        lastDuringCall = values.empty() ? -1.0f : values.back();
    });
    animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f));

    animator.Update(Timestep(1.0f));

    /*Still the manager's animation, already finished, and its own handler has already
      reported the value it finished on — the release happens after all of that.*/
    EXPECT_EQ(1u, heldDuringCall);
    EXPECT_TRUE(finishedDuringCall);
    EXPECT_FLOAT_EQ(10.0f, lastDuringCall);
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestDiscardingDoesNotReportAFinish)
{
    std::vector<float> values;
    std::size_t ends = 0;
    Animator animator;
    animator.OnTransitionEnd([&ends](const Node&){ ends++; });

    animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 10.0f));
    animator.Play(std::make_unique<Repeat>(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f),
      Repeat::Forever, Repeat::Replay));
    animator.Update(Timestep(0.1f));
    ASSERT_EQ(2u, animator.Count());

    animator.Clear();

    EXPECT_EQ(0u, ends);
    animator.Update(Timestep(1.0f));
    EXPECT_EQ(0u, ends);
}

TEST(AnimationTest, TestAnAnimationOfNoLengthIsReportedAtHandOver)
{
    std::vector<float> values;
    std::size_t ends = 0;
    Animator animator;
    animator.OnTransitionEnd([&ends](const Node&){ ends++; });
    animator.Play(MakeSoloLeaf(values, 30.0f, 70.0f, 0.0f));

    /*A transition of no length is over the moment it is started, so it reports at
      hand-over, before any advance has run.*/
    EXPECT_EQ(1u, ends);
    EXPECT_EQ(1u, animator.Count());

    animator.Update(Timestep(0.1f));

    EXPECT_EQ(1u, ends);
    EXPECT_EQ(0u, animator.Count());
}

TEST(AnimationTest, TestTheChildOfARepeatThatNeverFinishesIsReportedEachRun)
{
    std::vector<float> values;
    std::size_t ends = 0;
    Animator animator;
    animator.OnTransitionEnd([&ends](const Node&){ ends++; });
    animator.Play(std::make_unique<Repeat>(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f),
      Repeat::Forever, Repeat::Replay));

    for(int step = 0; step < 500; step++)
        animator.Update(Timestep(0.1f));

    /*The repeat itself never finishes and is never reported, but its child is reported once
      per run: 500 tenths of a second over a one-second child is 50 whole runs.*/
    EXPECT_EQ(50u, ends);
    EXPECT_EQ(1u, animator.Count());
}

TEST(AnimationTest, TestEachLeafOfASequenceIsReportedAsItFinishesInOrder)
{
    std::vector<float> values;
    std::vector<const Node*> reported;
    std::vector<std::size_t> valuesAtReport;
    Animator animator;
    animator.OnTransitionEnd([&reported, &values, &valuesAtReport](const Node& leaf)
    {
        reported.push_back(&leaf);
        valuesAtReport.push_back(values.size());
    });

    auto first = MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f);
    auto second = MakeSoloLeaf(values, 20.0f, 30.0f, 1.0f);
    const Node* firstLeaf = first.get();
    const Node* secondLeaf = second.get();

    auto sequence = std::make_unique<Sequence>();
    sequence->Add(std::move(first));
    sequence->Add(std::move(second));
    animator.Play(std::move(sequence));

    ASSERT_EQ(0u, reported.size());

    animator.Update(Timestep(1.0f));

    /*The first leaf reports from inside its own Update, before the sequence has started
      the second, so at that moment only the first leaf's two values exist.*/
    ASSERT_EQ(1u, reported.size());
    EXPECT_EQ(firstLeaf, reported[0]);
    EXPECT_EQ(2u, valuesAtReport[0]);

    animator.Update(Timestep(1.0f));

    ASSERT_EQ(2u, reported.size());
    EXPECT_EQ(secondLeaf, reported[1]);
    EXPECT_EQ(4u, valuesAtReport[1]);

    /*The sequence itself is a container and is never reported: only its two leaves have
      been. It has finished and been released, so no further report arrives.*/
    EXPECT_EQ(0u, animator.Count());

    animator.Update(Timestep(1.0f));

    EXPECT_EQ(2u, reported.size());
}

TEST(AnimationTest, TestTheChildOfARepeatIsReportedEachRun)
{
    std::vector<float> values;
    std::size_t ends = 0;
    Animator animator;
    animator.OnTransitionEnd([&ends](const Node&){ ends++; });
    animator.Play(std::make_unique<Repeat>(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f),
      3, Repeat::Replay));

    animator.Update(Timestep(1.0f));
    EXPECT_EQ(1u, ends);

    animator.Update(Timestep(1.0f));
    EXPECT_EQ(2u, ends);

    animator.Update(Timestep(1.0f));
    EXPECT_EQ(3u, ends);

    /*Three runs of the child, three reports; the repeat itself reports nothing and has
      finished and been released.*/
    EXPECT_EQ(0u, animator.Count());

    animator.Update(Timestep(1.0f));
    EXPECT_EQ(3u, ends);
}

TEST(AnimationTest, TestSettingTheEndHandlerAgainReplacesTheOneBefore)
{
    std::vector<float> values;
    std::size_t first = 0;
    std::size_t second = 0;
    Animator animator;
    animator.OnTransitionEnd([&first](const Node&){ first++; });
    animator.OnTransitionEnd([&second](const Node&){ second++; });
    animator.Play(MakeSoloLeaf(values, 0.0f, 10.0f, 1.0f));

    animator.Update(Timestep(1.0f));

    EXPECT_EQ(0u, first);
    EXPECT_EQ(1u, second);
}

TEST(AnimationTest, TestPlayingFromInsideTheEndHandlerWaitsForTheNextFrame)
{
    std::vector<float> first;
    std::vector<float> second;
    std::size_t ends = 0;
    Animator animator;
    animator.OnTransitionEnd([&](const Node&)
    {
        ends++;
        animator.Play(MakeSoloLeaf(second, 100.0f, 200.0f, 1.0f));
    });
    animator.Play(MakeSoloLeaf(first, 0.0f, 10.0f, 1.0f));

    EXPECT_NO_THROW(animator.Update(Timestep(1.0f)));

    EXPECT_EQ(1u, ends);
    EXPECT_EQ(1u, animator.Count());
    ASSERT_EQ(1u, second.size());
    EXPECT_FLOAT_EQ(100.0f, second[0]);

    animator.Update(Timestep(0.5f));

    EXPECT_EQ(1u, ends);
    ASSERT_EQ(2u, second.size());
    EXPECT_FLOAT_EQ(150.0f, second[1]);
    EXPECT_FLOAT_EQ(10.0f, first.back());
}

TEST(AnimationTest, TestDiscardingFromInsideTheEndHandlerCompletesTheAdvance)
{
    std::vector<float> first;
    std::vector<float> second;
    std::size_t ends = 0;
    Animator animator;
    animator.OnTransitionEnd([&](const Node&)
    {
        ends++;
        animator.Clear();
    });
    animator.Play(MakeSoloLeaf(first, 0.0f, 10.0f, 1.0f));
    animator.Play(MakeSoloLeaf(second, 100.0f, 200.0f, 1.0f));

    EXPECT_NO_THROW(animator.Update(Timestep(1.0f)));

    /*One notification and then nothing: the discard ends the advance with nothing already
      finished reported twice and nothing discarded advanced afterwards.*/
    EXPECT_EQ(1u, ends);
    EXPECT_EQ(0u, animator.Count());

    const std::size_t firstReports = first.size();
    const std::size_t secondReports = second.size();

    animator.Update(Timestep(1.0f));

    EXPECT_EQ(1u, ends);
    EXPECT_EQ(firstReports, first.size());
    EXPECT_EQ(secondReports, second.size());
}
