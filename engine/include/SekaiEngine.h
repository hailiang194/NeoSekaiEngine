/**
 * @file SekaiEngine.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief The header which includes all the needed headers for game engine
 * @version 1.0
 * @date 2026-10-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_SEKAI_ENGINE_H_
#define _SEKAI_ENGINE_SEKAI_ENGINE_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Application.h"
#include "SekaiEngine/Window.h"
#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Event/WindowEvent.h"
#include "SekaiEngine/Event/ApplicationEvent.h"
#include "SekaiEngine/Layer/Layer.h"
#include "SekaiEngine/Layer/LayerStack.h"
#include "SekaiEngine/Input.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Render/RendererAPI.h"
#include "SekaiEngine/Render/Color.h"
#include "SekaiEngine/Render/RenderCommand.h"
#include "SekaiEngine/Render/Renderer.h"
#include "SekaiEngine/Render/Camera.h"
#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Render/Texture.h"
#include "SekaiEngine/Shape/Shape.h"
#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Render/RenderProperties.h"
#include "SekaiEngine/Render/DrawCmd.h"
#include "SekaiEngine/Math/Utility.h"
#include "SekaiEngine/Render/Font.h"
#include "SekaiEngine/Math/Collision.h"
#include "SekaiEngine/EntryPoint.h"
#include "SekaiEngine/Object/UI.h"
#include "SekaiEngine/Object/TextureUI.h"
#include "SekaiEngine/Object/CircleUI.h"
#include "SekaiEngine/Object/RectangleUI.h"
#include "SekaiEngine/Object/GroupUI.h"
#include "SekaiEngine/Audio/Device.h"
#include "SekaiEngine/Audio/Sound.h"
#include "SekaiEngine/Audio/MusicStream.h"
#include "SekaiEngine/TextEngine/TextEngine.h"
#include "SekaiEngine/Log.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Linear.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"
#include "SekaiEngine/Animation/Transitions/Quad.h"
#include "SekaiEngine/Animation/Transitions/Cubic.h"
#include "SekaiEngine/Animation/Transitions/Quart.h"
#include "SekaiEngine/Animation/Transitions/Quint.h"
#include "SekaiEngine/Animation/Transitions/Sine.h"
#include "SekaiEngine/Animation/Transitions/Expo.h"
#include "SekaiEngine/Animation/Transitions/Circ.h"
#include "SekaiEngine/Animation/Transitions/Back.h"
#include "SekaiEngine/Animation/Transitions/Elastic.h"
#include "SekaiEngine/Animation/Transitions/Bounce.h"
#include "SekaiEngine/Animation/Transitions/Types.h"

#include "version.h"

#endif //!_SEKAI_ENGINE_SEKAI_ENGINE_H_
