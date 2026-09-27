/**
 * @file frame_threading.cpp
 * @brief Headless coverage of the handoff between the updating thread and the
 * thread that replays a frame's recorded draws
 *
 * These tests deliberately never call InitWindow, BeginDrawing or EndDrawing:
 * engine_test runs with no display. What they cover is the handoff's data
 * contract, which needs no graphics context. Application::Run, the permit, the
 * parking, the close path, and the input and event threading all need a window
 * and are covered by running the examples, not by this suite.
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <vector>

#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Render/DrawCmd.h"
#include "SekaiEngine/Render/RenderCommand.h"

namespace
{
    /**
     * @brief A rectangle draw that is identifiable by its size alone
     */
    SekaiEngine::Render::DrawCmd MarkerRect(const float& size)
    {
        SekaiEngine::Render::RenderProperties props;
        return SekaiEngine::Render::MakeRectCmd(props,
            SekaiEngine::Shape::Rectangle(SekaiEngine::Math::Vector2D(0.0f, 0.0f), size, size)
        );
    }

    /**
     * @brief A circle draw that is identifiable by its radius alone
     */
    SekaiEngine::Render::DrawCmd MarkerCircle(const float& radius)
    {
        SekaiEngine::Render::RenderProperties props;
        return SekaiEngine::Render::MakeCircleCmd(props,
            SekaiEngine::Shape::Circle(SekaiEngine::Math::Vector2D(0.0f, 0.0f), radius)
        );
    }

    /**
     * @brief A text draw that is identifiable by the string it names
     *
     * @note The string is a literal, so it outlives the frame's command buffer as
     * TextDraw requires.
     */
    SekaiEngine::Render::DrawCmd MarkerText(const char* text)
    {
        SekaiEngine::Render::RenderProperties props;
        return SekaiEngine::Render::MakeTextCmd(props, text,
            SekaiEngine::Math::Vector2D(0.0f, 0.0f), "noto"
        );
    }
} // namespace

//Order survives recording on one thread and replay on another
TEST(FrameThreadingTest, RecordedOrderSurvivesHandoff)
{
    std::vector<SekaiEngine::Render::DrawCmd> buffer;

    std::thread updater([&buffer]()
    {
        SekaiEngine::Render::RenderCommand::BeginRecording(buffer);
        SekaiEngine::Render::RenderCommand::Record(MarkerRect(10.0f));
        SekaiEngine::Render::RenderCommand::Record(MarkerCircle(20.0f));
        SekaiEngine::Render::RenderCommand::Record(MarkerText("first"));
        SekaiEngine::Render::RenderCommand::Record(MarkerRect(40.0f));
        SekaiEngine::Render::RenderCommand::EndRecording();
    });
    updater.join();

    ASSERT_EQ(buffer.size(), 4u);

    EXPECT_EQ(buffer[0].tag, SekaiEngine::Render::DrawTag::Rectangle);
    EXPECT_FLOAT_EQ(buffer[0].variant.rect.rect.Width, 10.0f);
    EXPECT_EQ(buffer[1].tag, SekaiEngine::Render::DrawTag::Circle);
    EXPECT_FLOAT_EQ(buffer[1].variant.circle.circle.Radius, 20.0f);
    EXPECT_EQ(buffer[2].tag, SekaiEngine::Render::DrawTag::Text);
    EXPECT_STREQ(buffer[2].variant.text.text, "first");
    EXPECT_EQ(buffer[3].tag, SekaiEngine::Render::DrawTag::Rectangle);
    EXPECT_FLOAT_EQ(buffer[3].variant.rect.rect.Width, 40.0f);
}

//A frame's draws are not mixed with another frame's, and consecutive frames
//write to different buffers
TEST(FrameThreadingTest, ConsecutiveFrameBuffersDoNotMix)
{
    std::vector<SekaiEngine::Render::DrawCmd> frameBuffer[2];

    //Frame 0, recorded on the updating thread and handed over
    std::thread updater([&frameBuffer]()
    {
        SekaiEngine::Render::RenderCommand::BeginRecording(frameBuffer[0]);
        SekaiEngine::Render::RenderCommand::Record(MarkerRect(1.0f));
        SekaiEngine::Render::RenderCommand::Record(MarkerRect(2.0f));
        SekaiEngine::Render::RenderCommand::EndRecording();
    });
    updater.join();

    //Frame 1 goes to the other buffer, while frame 0 is still being replayed
    SekaiEngine::Render::RenderCommand::BeginRecording(frameBuffer[1]);
    SekaiEngine::Render::RenderCommand::Record(MarkerCircle(3.0f));
    SekaiEngine::Render::RenderCommand::EndRecording();

    ASSERT_EQ(frameBuffer[0].size(), 2u);
    EXPECT_FLOAT_EQ(frameBuffer[0][0].variant.rect.rect.Width, 1.0f);
    EXPECT_FLOAT_EQ(frameBuffer[0][1].variant.rect.rect.Width, 2.0f);

    ASSERT_EQ(frameBuffer[1].size(), 1u);
    EXPECT_EQ(frameBuffer[1][0].tag, SekaiEngine::Render::DrawTag::Circle);
    EXPECT_FLOAT_EQ(frameBuffer[1][0].variant.circle.circle.Radius, 3.0f);

    //And frame 0's buffer is empty again once its draws have been replayed
    frameBuffer[0].clear();
    EXPECT_TRUE(frameBuffer[0].empty());
}

//Recording between frames is reported rather than buffered
TEST(FrameThreadingTest, RecordOutsideRecordingScopeIsReportedAndDropped)
{
    testing::internal::CaptureStderr();
    SekaiEngine::Render::RenderCommand::Record(MarkerRect(5.0f));
    std::string report = testing::internal::GetCapturedStderr();

    EXPECT_NE(report.find("outside a recording scope"), std::string::npos);

    std::vector<SekaiEngine::Render::DrawCmd> frameBuffer[2];
    SekaiEngine::Render::RenderCommand::BeginRecording(frameBuffer[0]);
    SekaiEngine::Render::RenderCommand::EndRecording();
    EXPECT_TRUE(frameBuffer[0].empty());
}
