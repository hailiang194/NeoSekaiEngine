#include "SekaiEngine/Render/RenderCommand.h"
#include "SekaiEngine/Application.h"
#include "SekaiEngine/TextEngine/TextEngine.h"
#include <iostream>

namespace SekaiEngine
{
    namespace Render
    {
        namespace RenderCommand
        {
            namespace
            {
                /**
                 * @brief The buffer Record appends to, set by BeginRecording
                 *
                 * @note Thread local because the buffer belongs to the thread recording
                 * the frame: the update thread and the thread replaying the frame are two
                 * threads, and only the recording one ever holds this.
                 */
                thread_local std::vector<DrawCmd>* g_recording = nullptr;
            }

            void BeginRecording(std::vector<DrawCmd>& commands)
            {
                g_recording = &commands;
            }

            void EndRecording()
            {
                g_recording = nullptr;
            }

            void Record(const DrawCmd& command)
            {
                if(g_recording == nullptr)
                {
                    std::cerr << "SekaiEngine: dropped a draw recorded outside a recording scope"
                        << std::endl;
                    return;
                }
                g_recording->push_back(command);
            }

            void ReplayFrame(const std::vector<DrawCmd>& commands, const Color& clearColor)
            {
                API::BeginDrawing();
                API::SetClearColor(clearColor);
                API::Clear();

                for(std::vector<DrawCmd>::const_iterator iter = commands.begin(); iter != commands.end(); ++iter)
                {
                    switch(iter->tag)
                    {
                        case DrawTag::Circle:
                            API::DrawCircle(
                                iter->variant.circle.circle, iter->tint,
                                iter->variant.circle.startAngle, iter->variant.circle.endAngle,
                                iter->variant.circle.segment
                            );
                            break;
                        case DrawTag::Rectangle:
                            API::DrawRect(
                                iter->variant.rect.rect, iter->tint,
                                iter->variant.rect.origin, iter->variant.rect.rotation
                            );
                            break;
                        case DrawTag::Texture:
                            API::DrawTexture(
                                iter->variant.tex.tex, iter->tint,
                                iter->variant.tex.source, iter->variant.tex.dest,
                                iter->variant.tex.origin, iter->variant.tex.rotation
                            );
                            break;
                        case DrawTag::Text:
                        {
                            Application* app = Application::Instance();
                            if(app != nullptr)
                            {
                                //Glyph rasterisation and glyph texture creation happen here,
                                //on the thread replaying the frame, not while recording.
                                app->TextEngine().DrawText(
                                    iter->variant.text.text, iter->variant.text.position,
                                    iter->tint, iter->variant.text.fontName
                                );
                            }
                            break;
                        }
                        default:
                            std::cerr << "SekaiEngine: skipped a draw command of unrecognised kind "
                                << static_cast<int>(iter->tag) << std::endl;
                            break;
                    }
                }

                API::EndDrawing();
            }
        } // namespace RenderCommand

    } // namespace Render

} // namespace SekaiEngine
