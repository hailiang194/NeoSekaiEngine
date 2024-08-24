#ifndef _SEKAI_ENGINE_OBJECT_GROUP_U_H_
#define _SEKAI_ENGINE_OBJECT_GROUP_U_H_

#include "UI.h"
#include <vector>

namespace SekaiEngine
{
    namespace Object
    {
        class GroupUI: public UI
        {
        public:
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param parent the pointer of parent UI
             * @param self the render properties of the object
             */
            EXTENDAPI GroupUI(UI* parent = nullptr, Render::RenderProperties self = Render::RenderProperties());
            
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param ui copied object
             */
            EXTENDAPI GroupUI(const GroupUI& ui);

            /**
             * @brief Copied assignment operator
             * 
             * @param ui Copied object
             * @return GroupUI& the reference of the object itself
             */
            EXTENDAPI GroupUI& operator=(const GroupUI& ui);

            /**
             * @brief Destroy the Texture UI object
             * 
             */
            virtual EXTENDAPI ~GroupUI();

            /**
             * @brief Handle update
             * 
             * @param elipse the elipse time of last frame
             */
            virtual EXTENDAPI void OnUpdate(const Timestep& elipse) override;

            /**
             * @brief Handle render
             * 
             */
            virtual EXTENDAPI void OnRender() override;

            /**
             * @brief Handle event
             * 
             * @param event event need to be handled
             */
            virtual EXTENDAPI void OnEvent(Event::Event& event) override;

            /**
             * @brief Set the Position of the object
             * 
             * @param position new position
             */
            EXTENDAPI void SetPosition(const Math::Vector2D& position) override;

            /**
             * @brief Set the Scale of the object
             * 
             * @param scale new scale
             */
            EXTENDAPI void SetScale(const Math::Vector2D& scale) override;

            /**
             * @brief Set the Tint for object
             * 
             * @param tint new tint
             */
            EXTENDAPI void SetTint(const Render::Color& tint) override;

            /**
             * @brief Set the Origin rotation for object
             * 
             * @param origin new origin
             */
            EXTENDAPI void SetOrigin(const Math::Vector2D& origin) override;

            /**
             * @brief Set the Rotation in degree for object
             * 
             * @param rotation new rotation in degree
             */
            EXTENDAPI void SetRotation(const float& rotation) override;
        protected:
            std::vector<UI*> m_children;
        };
    } // namespace Object
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_OBJECT_GROUP_U_H_