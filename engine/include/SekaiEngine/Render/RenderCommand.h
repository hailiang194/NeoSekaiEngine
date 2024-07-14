/**
 * @file RenderCommand.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the command for rendering
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_
#define _SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/RendererAPI.h"
#include "SekaiEngine/Render/Params/CircleRenderParams.h"
#include "SekaiEngine/Render/Params/RectangleRenderParams.h"
#include "SekaiEngine/Render/Params/TextureRenderParams.h"

namespace SekaiEngine
{
    namespace Render
    {
        namespace RenderCommand
        {

            /**
             * @brief Startup rendering for a frame
             * 
             * @param color background color
             */
            void StartDrawing(const Color& color);

            /**
             * @brief Finish rendering for a frame
             * 
             */
            void FinishDrawing();

            /**
             * @brief Get the Bounding Box object
             * 
             * @param param the parameters for rendering a circle
             * @return Shape::Rectangle the bounding box of circle
             */
            EXTENDAPI Shape::Rectangle GetBoundingBox(const CircleRenderParams& param);

            /**
             * @brief Get the Bounding Box object
             * 
             * @param param the paramters for rendering the rectangle
             * @return Shape::Rectangle the bounding box of rectangle
             */
            EXTENDAPI Shape::Rectangle GetBoundingBox(const RectangleRenderParams& param);

            /**
             * @brief Get the Bounding Box object
             * 
             * @param param the parameters for rendering the texture
             * @return Shape::Rectangle the bounding box of texture
             */
            EXTENDAPI Shape::Rectangle GetBoundingBox(const TextureRenderParams& param);

            /**
             * @brief Render the parameters
             * 
             * @param params the parameters
             */
            void Render(const RenderParams& params);

            /**
             * @brief Render the circle
             * 
             * @param params the parameters for rendering circle
             */
            void Render(const CircleRenderParams& params);

            /**
             * @brief Render the rectangle
             * 
             * @param params the parameters for rendering rectangle
             */
            void Render(const RectangleRenderParams& params);

            /**
             * @brief Render the texture
             * 
             * @param params the paramters for rendering texture
             */
            void Render(const TextureRenderParams& params);

            inline void StartDrawing(const Color& color)
            {
                Render::API::BeginDrawing();
                Render::API::SetClearColor(color);
                Render::API::Clear();
            }

            inline void FinishDrawing()
            {
                Render::API::EndDrawing();
            }

            inline void Render(const RenderParams& params)
            {
                if(dynamic_cast<const CircleRenderParams*>(&params) != nullptr)
                {
                    Render(*dynamic_cast<const CircleRenderParams*>(&params));
                }
                else if(dynamic_cast<const RectangleRenderParams*>(&params) != nullptr)
                {
                    Render(*dynamic_cast<const RectangleRenderParams*>(&params));
                }
                else if(dynamic_cast<const TextureRenderParams*>(&params) != nullptr)
                {
                    Render(*dynamic_cast<const TextureRenderParams*>(&params));
                }
            }

            inline void Render(const CircleRenderParams& params)
            {
                Render::API::DrawCircle(
                    params.DrawCircle(), params.Tint(), 
                    params.StartAngle(), params.EndAngle(), params.Segment()
                );
            }

            inline void Render(const RectangleRenderParams& params)
            {
                Render::API::DrawRect(params.DrawRect(), params.Tint(), params.Origin(), params.Rotation());
            }

            inline void Render(const TextureRenderParams& params)
            {
                Render::API::DrawTexture(params.Texture(), params.Tint(), 
                    params.Source(), params.Destination(), 
                    params.Origin(), params.Rotation()
                );
            }

        } // namespace RenderCommand
        
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_