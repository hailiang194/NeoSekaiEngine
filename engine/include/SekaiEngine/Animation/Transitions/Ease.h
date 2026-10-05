#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_EASE_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_EASE_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief Which end of a transition a curve spends its shape at
     *
     * @note The three directions of the easings.net catalogue. They are named once here
     * and shared by every easing family, so a mode means the same thing wherever it is
     * used.
     */
    class EXTENDAPI Ease
    {
    public:
      enum Mode
      {
        In, /*!< The whole shape is spent at the start of the duration*/
        Out, /*!< The whole shape is spent at the end of the duration*/
        InOut /*!< The shape is split evenly across both ends*/
      };
    };
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_EASE_H_