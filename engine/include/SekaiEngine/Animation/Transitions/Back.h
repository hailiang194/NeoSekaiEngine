#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_BACK_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_BACK_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /*!< How far past its end the catalogue's Back curve travels, and the cubic coefficient
      that travels it. The InOut form uses a larger overshoot still, so it has its own.*/
    static const float BACK_OVERSHOOT = 1.70158f;
    static const float BACK_OVERSHOOT_CUBIC = BACK_OVERSHOOT + 1.0f;
    static const float BACK_INOUT_OVERSHOOT = BACK_OVERSHOOT * 1.525f;
    static const float BACK_INOUT_OVERSHOOT_CUBIC = BACK_INOUT_OVERSHOOT + 1.0f;

    /**
     * @brief The back easing, a curve that travels past its end before settling
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Back: public Transition<Back>
    {
    public:
      /**
       * @brief Build a back transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Back(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Back(const Back& back) = default;
      Back& operator=(const Back& back) = default;
      Back(Back&& back) = default;
      Back& operator=(Back&& back) = default;
      ~Back() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover, which for this family is
       * deliberately greater than one part way through the duration
       */
      float GetProgressValue(const float& timeRatio) const;

    protected:
      Ease::Mode m_mode; /*!< The direction this transition eases in*/
    };


    inline Back::Back(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Back>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Back::GetProgressValue(const float& timeRatio) const
    {
      /*This is one of the two families the catalogue defines to overshoot: the value leaves
        its start in the wrong direction, or arrives early, and then settles back onto the
        end value. It still lands exactly on that end value, which is what keeps it inside
        the transition's own contract.*/
      switch(m_mode)
      {
        case Ease::Out:
          return 1.0f + BACK_OVERSHOOT_CUBIC * std::pow(timeRatio - 1.0f, 3.0f) +
            BACK_OVERSHOOT * std::pow(timeRatio - 1.0f, 2.0f);
        case Ease::InOut:
          /*Two overshoots, each run over its own half of the duration.*/
          return timeRatio < 0.5f ?
            (std::pow(2.0f * timeRatio, 2.0f) * ((BACK_INOUT_OVERSHOOT_CUBIC) * 2.0f * timeRatio - BACK_INOUT_OVERSHOOT)) / 2.0f :
            (std::pow(2.0f * timeRatio - 2.0f, 2.0f) * ((BACK_INOUT_OVERSHOOT_CUBIC) * (timeRatio * 2.0f - 2.0f) + BACK_INOUT_OVERSHOOT) + 2.0f) / 2.0f;
        case Ease::In:
        default:
          return BACK_OVERSHOOT_CUBIC * timeRatio * timeRatio * timeRatio - BACK_OVERSHOOT * timeRatio * timeRatio;
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_BACK_H_