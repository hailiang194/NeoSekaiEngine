#include "SekaiEngine/Render/RenderProperties.h"


namespace SekaiEngine
{
    namespace Render
    {
        RenderProperties::RenderProperties()
            :Offset(0.0f, 0.0f), Scale(1.0f, 1.0f), Tint(0xffffffff), Origin(0.0f, 0.0f), Rotation(0.0f) 
        {

        }

        RenderProperties::RenderProperties(const RenderProperties& props)
            :Offset(props.Offset), Scale(props.Scale), Tint(props.Tint), Origin(props.Origin), Rotation(props.Rotation)
        {

        }

        RenderProperties& RenderProperties::operator=(const RenderProperties& props)
        {
            Offset = props.Offset;
            Scale = props.Scale;
            Tint = props.Tint;
            Origin = props.Origin;
            Rotation = props.Rotation;

            return (*this);
        }

        RenderProperties::~RenderProperties()
        {

        }

        void UpdateRenderProperies(const RenderProperties &parent, const RenderProperties &self, RenderProperties render)
        {
            render.Offset.X() = parent.Offset.X() + parent.Scale.X() * self.Offset.X();
            render.Offset.Y() = parent.Offset.Y() + parent.Scale.Y() * self.Offset.Y();

            render.Scale.X() = parent.Scale.X() * self.Scale.X();
            render.Scale.Y() = parent.Scale.Y() * self.Scale.Y();

            ColorParser parser;
            parser.value.r = static_cast<uint8_t>(parent.Tint.r() * self.Tint.r() / 255);
            parser.value.g = static_cast<uint8_t>(parent.Tint.g() * self.Tint.g() / 255);
            parser.value.b = static_cast<uint8_t>(parent.Tint.b() * self.Tint.b() / 255);
            parser.value.a = static_cast<uint8_t>(parent.Tint.a() * self.Tint.a() / 255);

            render.Tint = (Render::Color)parser.code;

            render.Origin.X() = parent.Origin.X() + parent.Scale.X() * self.Origin.X();
            render.Origin.Y() = parent.Origin.Y() + parent.Scale.Y() * self.Origin.Y();

            render.Rotation = parent.Rotation + self.Rotation;
        }
    } // namespace Render

    
} // namespace SekaiEngine
