#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_EXPO_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_EXPO_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The exponential easing, a base-two power of ten
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Expo: public Transition<Expo>
    {
    public:
      /**
       * @brief Build an exponential transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Expo(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Expo(const Expo& expo) = default;
      Expo& operator=(const Expo& expo) = default;
      Expo(Expo&& expo) = default;
      Expo& operator=(Expo&& expo) = default;
      ~Expo() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover
       */
      float GetProgressValue(const float& timeRatio) const;

    protected:
      Ease::Mode m_mode; /*!< The direction this transition eases in*/
    };


    inline Expo::Expo(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Expo>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Expo::GetProgressValue(const float& timeRatio) const
    {
      /*This family is flat at both ends: its rate of change is zero at the start and at
        the end, which is the whole point of choosing it over a plain power. The raw
        formula leaves both ends at a rounded 0.0009 and 0.999, so the two ends are
        returned directly instead.*/
      switch(m_mode)
      {
        case Ease::Out:
          if(timeRatio == 1.0f)
            return 1.0f;
          return 1.0f - std::pow(2.0f, -10.0f * timeRatio);
        case Ease::InOut:
          if(timeRatio == 0.0f)
            return 0.0f;
          if(timeRatio == 1.0f)
            return 1.0f;
          return timeRatio < 0.5f ?
            std::pow(2.0f, 20.0f * timeRatio - 10.0f) / 2.0f :
            (2.0f - std::pow(2.0f, -20.0f * timeRatio + 10.0f)) / 2.0f;
        case Ease::In:
        default:
          if(timeRatio == 0.0f)
            return 0.0f;
          return std::pow(2.0f, 10.0f * timeRatio - 10.0f);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_EXPO_H_