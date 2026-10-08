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
     * The direction is a template parameter too, so the three curves of this family share
     * one body and an instantiation is left holding only the shape it was asked for.
     *
     * @tparam MODE the direction the shape is spent at, one of Ease::In, Ease::Out or
     * Ease::InOut
     */
    template<Ease::Mode MODE>
    class EXTENDAPI Back: public Transition<Back<MODE>>
    {
    public:
      /**
       * @brief Build a back transition that eases in the direction its type names
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Back(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Back(const Back<MODE>& back) = default;
      Back& operator=(const Back<MODE>& back) = default;
      Back(Back<MODE>&& back) = default;
      Back& operator=(Back<MODE>&& back) = default;
      ~Back() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover, which for this family is
       * deliberately greater than one part way through the duration
       */
      float GetProgressValue(const float& timeRatio) const;

      /*Each direction is the family's own algebra on a different argument, so only the
        argument changes between them. Checked here rather than left to the chain below,
        whose final else would quietly build any other value as In.*/
      static_assert(MODE == Ease::In || MODE == Ease::Out || MODE == Ease::InOut,
        "Ease::Mode must name one of the catalogue's three directions");
    };


    template<Ease::Mode MODE>
    inline Back<MODE>::Back(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Back<MODE>>(start, end, duration, handler, total)
    {
    }

    template<Ease::Mode MODE>
    inline float Back<MODE>::GetProgressValue(const float& timeRatio) const
    {
      /*This is one of the two families the catalogue defines to overshoot: the value leaves
        its start in the wrong direction, or arrives early, and then settles back onto the
        end value. It still lands exactly on that end value, which is what keeps it inside
        the transition's own contract.*/
      if constexpr(MODE == Ease::Out)
      {
        return 1.0f + BACK_OVERSHOOT_CUBIC * std::pow(timeRatio - 1.0f, 3.0f) +
          BACK_OVERSHOOT * std::pow(timeRatio - 1.0f, 2.0f);
      }
      else if constexpr(MODE == Ease::InOut)
      {
        /*Two overshoots, each run over its own half of the duration.*/
        return timeRatio < 0.5f ?
          (std::pow(2.0f * timeRatio, 2.0f) * ((BACK_INOUT_OVERSHOOT_CUBIC) * 2.0f * timeRatio - BACK_INOUT_OVERSHOOT)) / 2.0f :
          (std::pow(2.0f * timeRatio - 2.0f, 2.0f) * ((BACK_INOUT_OVERSHOOT_CUBIC) * (timeRatio * 2.0f - 2.0f) + BACK_INOUT_OVERSHOOT) + 2.0f) / 2.0f;
      }
      else
      {
        return BACK_OVERSHOOT_CUBIC * timeRatio * timeRatio * timeRatio - BACK_OVERSHOOT * timeRatio * timeRatio;
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_BACK_H_