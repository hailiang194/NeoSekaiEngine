#include "SekaiEngine/Render/DrawCmd.h"

namespace SekaiEngine
{
    namespace Render
    {
        DrawCmd MakeCircleCmd(const RenderProperties& props, const Shape::Circle& circle,
            const float& startAngle, const float& endAngle, const int& segment
        )
        {
            DrawCmd command;
            command.tag = DrawTag::Circle;
            command.tint = props.Tint;
            command.variant.circle.circle = Shape::Circle(
                Math::Vector2D(
                    props.Offset.X() + circle.Center.X() * props.Scale.X(),
                    props.Offset.Y() + circle.Center.Y() * props.Scale.Y()
                ),
                circle.Radius * props.Scale.X()
            );
            command.variant.circle.startAngle = startAngle;
            command.variant.circle.endAngle = endAngle;
            command.variant.circle.segment = segment;
            return command;
        }

        DrawCmd MakeRectCmd(const RenderProperties& props, const Shape::Rectangle& rect)
        {
            DrawCmd command;
            command.tag = DrawTag::Rectangle;
            command.tint = props.Tint;
            command.variant.rect.rect = Shape::Rectangle(
                Math::Vector2D(
                    props.Offset.X() + rect.Position.X() * props.Scale.X(),
                    props.Offset.Y() + rect.Position.Y() * props.Scale.Y()
                ),
                rect.Width * props.Scale.X(), rect.Height * props.Scale.Y()
            );
            command.variant.rect.origin = props.Origin;
            command.variant.rect.rotation = props.Rotation;
            return command;
        }

        DrawCmd MakeTexCmd(const RenderProperties& props, const Texture& texture,
            const Math::Vector2D& position, const Shape::Rectangle& atlasPosition
        )
        {
            DrawCmd command;
            command.tag = DrawTag::Texture;
            command.tint = props.Tint;
            command.variant.tex.source = atlasPosition;
            command.variant.tex.dest = Shape::Rectangle(
                Math::Vector2D(
                    props.Offset.X() + position.X() * props.Scale.X(),
                    props.Offset.Y() + position.Y() * props.Scale.Y()
                ),
                texture.Width() * props.Scale.X(), texture.Height() * props.Scale.Y()
            );
            command.variant.tex.origin = props.Origin;
            command.variant.tex.rotation = props.Rotation;
            command.variant.tex.tex = texture;
            return command;
        }

        DrawCmd MakeTextCmd(const RenderProperties& props, const char* text,
            const Math::Vector2D& position, const char* fontName
        )
        {
            DrawCmd command;
            command.tag = DrawTag::Text;
            command.tint = props.Tint;
            command.variant.text.text = text;
            command.variant.text.position = Math::Vector2D(
                props.Offset.X() + position.X() * props.Scale.X(),
                props.Offset.Y() + position.Y() * props.Scale.Y()
            );
            command.variant.text.fontName = fontName;
            return command;
        }
    } // namespace Render

} // namespace SekaiEngine
