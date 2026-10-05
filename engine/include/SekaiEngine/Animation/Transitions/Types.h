#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_TYPES_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_TYPES_H_

#include <variant>
#include "SekaiEngine/Animation/Transitions/Back.h"
#include "SekaiEngine/Animation/Transitions/Bounce.h"
#include "SekaiEngine/Animation/Transitions/Circ.h"
#include "SekaiEngine/Animation/Transitions/Cubic.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"
#include "SekaiEngine/Animation/Transitions/Elastic.h"
#include "SekaiEngine/Animation/Transitions/Expo.h"
#include "SekaiEngine/Animation/Transitions/Linear.h"
#include "SekaiEngine/Animation/Transitions/Quad.h"
#include "SekaiEngine/Animation/Transitions/Quart.h"
#include "SekaiEngine/Animation/Transitions/Quint.h"
#include "SekaiEngine/Animation/Transitions/Sine.h"

#define EASE_TRANSITION(name) name<Ease::In>, name<Ease::Out>, name<Ease::InOut>

namespace SekaiEngine {
  namespace Animation {
    using TransitionVariant = std::variant<
      EASE_TRANSITION(Back),
      EASE_TRANSITION(Bounce),
      EASE_TRANSITION(Circ),
      EASE_TRANSITION(Cubic),
      EASE_TRANSITION(Elastic),
      EASE_TRANSITION(Expo),
      Linear,
      EASE_TRANSITION(Quad),
      EASE_TRANSITION(Quart),
      EASE_TRANSITION(Quint),
      EASE_TRANSITION(Sine)
      >;
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_TYPES_H_
