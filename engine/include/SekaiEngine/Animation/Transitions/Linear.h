#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_LINEAR_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_LINEAR_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The linear transition, a value moving from its start to its end at a constant rate
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     */
    class EXTENDAPI Linear: public Transition<Linear> 
    {
    public:
      /*!< The base holds every piece of state a transition needs, so its constructor is
        inherited here. Without this the derived class only has the implicitly declared
        copy constructor, and no way to build a transition at all.*/
      using Transition<Linear>::Transition;

      Linear(const Linear& linear) = default;
      Linear& operator=(const Linear& linear) = default;
      Linear(Linear&& linear) = default;
      Linear& operator=(Linear&& linear) = default;
      ~Linear() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover, which for a linear transition
       * is the elapsed fraction itself
       */
      float GetProgressValue(const float& timeRatio) const;
    };


    inline float Linear::GetProgressValue(const float& timeRatio) const
    {
      return timeRatio;
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_LINEAR_H_