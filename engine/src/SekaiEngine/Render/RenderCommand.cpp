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
                 * @brief The frame's recorded draws, replayed in order by FinishDrawing
                 */
                std::vector<DrawCmd> g_commands;
            }

            void Record(const DrawCmd& command)
            {
                g_commands.push_back(command);
            }

            void FinishDrawing()
            {
                for(std::vector<DrawCmd>::iterator iter = g_commands.begin(); iter != g_commands.end(); ++iter)
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

                g_commands.clear();
                API::EndDrawing();
            }
        } // namespace RenderCommand

    } // namespace Render

} // namespace SekaiEngine
