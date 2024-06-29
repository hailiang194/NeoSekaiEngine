#include "SekaiEngine.h"
#include <iostream>

class ExampleLayer: public SekaiEngine::Layer::Layer
{
public:
    ExampleLayer()
        :Layer("Hello friend"), camera(), rect(SekaiEngine::Math::Vector2D(100, 100), 100, 50), 
        font("Arial", "./Arial.ttf"),
        circle(SekaiEngine::Math::Vector2D(100.0f, 100.0f), 5.0f), rotation(0.0f)
    {
        std::cout << "Height=" <<
        SekaiEngine::Application::Instance()->Window().GetHeight()
        << ", Width=" << SekaiEngine::Application::Instance()->Window().GetWidth() << std::endl;
    }

    ExampleLayer(const ExampleLayer& layer)
        :Layer(layer), camera(layer.camera), rect(layer.rect), font(layer.font)
    {
    }

    ~ExampleLayer()
    {

    }

    void OnUpdate(const SekaiEngine::Timestep& elipse) override
    {
        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_A))
            //rect.Position.X() -= (100 * elipse.ToSeconds());
            rotation -= 0.1f;
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_D))
            //rect.Position.X() += (100 * elipse.ToSeconds());
            rotation += 0.1f;

        if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_W))
            rect.Position.Y() -= (100 * elipse.ToSeconds());
        else if(SekaiEngine::Input::IsKeyPressed(SekaiEngine::Input::KEY_S))
            rect.Position.Y() += (100 * elipse.ToSeconds());

        if(SekaiEngine::Input::IsButtonPressed(SekaiEngine::Input::MouseKey::BUTTON_LEFT))
        {
            std::cout << "Mouse left clicked" << std::endl;
        }

    }

    void OnRender() override
    {
        
        camera.applyCamera();
        SekaiEngine::Render::RenderProperties props;
        props.Tint = 0x00ff00ff;
        props.Rotation = rotation;
        props.Origin = SekaiEngine::Math::Vector2D(rect.Width * 0.5f, rect.Height * 0.5f);
        //props.Offset = props.Origin;
        SekaiEngine::Render::RectangleRenderParams params(props, rect);
        SekaiEngine::Render::RenderCommand::Render(params);

        SekaiEngine::Shape::Rectangle bounding = SekaiEngine::Render::RenderCommand::GetBoundingBox(params);
        SekaiEngine::Render::API::DrawBounding(bounding);

        SekaiEngine::Render::CircleRenderParams circleParmas(props, circle);
        SekaiEngine::Render::RenderCommand::Render(circleParmas);

        SekaiEngine::Render::API::DrawText("Hello friend", SekaiEngine::Math::Vector2D(500, 500), (SekaiEngine::Render::Color)0x00ff00ff, 20, font);

        camera.unapplyCamera();
        
    }
private:
    SekaiEngine::Render::Camera2D camera;
    SekaiEngine::Shape::Rectangle rect;
    SekaiEngine::Render::Font font;
    SekaiEngine::Shape::Circle circle;
    float rotation;
};


class Sandbox: public SekaiEngine::Application
{
public:
    Sandbox()
        :Application()
    {
        PushLayer(new ExampleLayer());
    }

    Sandbox(const Sandbox& sandbox)
        :Application(sandbox)
    {

    }

    ~Sandbox()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new Sandbox();
}