#ifndef _SEKAI_ENGINE_OBJECT_RECTANGLE_UI_H_
#define _SEKAI_ENGINE_OBJECT_RECTANGLE_UI_H_

#include "UI.h"
#include "SekaiEngine/Shape/Rectangle.h"

namespace SekaiEngine
{
    namespace Object
    {
        class EXTENDAPI RectangleUI: public UI
        {
        public:
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param rectangle the rectangle of UI
             * @param parent the pointer of parent UI
             * @param self the render properties of the object
             */
            RectangleUI(const Shape::Rectangle& rectangle, UI* parent = nullptr, Render::RenderProperties self = Render::RenderProperties());
            
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param ui copied object
             */
            RectangleUI(const RectangleUI& ui);

            /**
             * @brief Copied assignment operator
             * 
             * @param ui Copied object
             * @return RectangleUI& the reference of the object itself
             */
            RectangleUI& operator=(const RectangleUI& ui);

            /**
             * @brief Destroy the Texture UI object
             * 
             */
            virtual ~RectangleUI();

            /**
             * @brief Handle update
             * 
             * @param elipse the elipse time of last frame
             */
            virtual void OnUpdate(const Timestep& elipse) override;

            /**
             * @brief Handle render
             * 
             */
            virtual void OnRender() override;

            /**
             * @brief Handle event
             * 
             * @param event event need to be handled
             */
            virtual void OnEvent(Event::Event& event) override;

            /**
             * @brief Set the Position of the object
             * 
             * @param position new position
             */
            void SetPosition(const Math::Vector2D& position) override;

            /**
             * @brief Set the Scale of the object
             * 
             * @param scale new scale
             */
            void SetScale(const Math::Vector2D& scale) override;

            /**
             * @brief Set the Tint for object
             * 
             * @param tint new tint
             */
            void SetTint(const Render::Color& tint) override;

            /**
             * @brief Set the Origin rotation for object
             * 
             * @param origin new origin
             */
            void SetOrigin(const Math::Vector2D& origin) override;

            /**
             * @brief Set the Rotation in degree for object
             * 
             * @param rotation new rotation in degree
             */
            void SetRotation(const float& rotation) override;
        protected:
            Shape::Rectangle m_rectangle;
        };
    } // namespace Object
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_OBJECT_RECTANGLE_UI_H_