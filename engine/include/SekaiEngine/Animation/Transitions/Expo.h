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
     * The direction is a template parameter too, so the three curves of this family share
     * one body and an instantiation is left holding only the shape it was asked for.
     *
     * @tparam MODE the direction the shape is spent at, one of Ease::In, Ease::Out or
     * Ease::InOut
     */
    template<Ease::Mode MODE>
    class EXTENDAPI Expo: public Transition<Expo<MODE>>
    {
    public:
      /**
       * @brief Build an exponential transition that eases in the direction its type names
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Expo(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Expo(const Expo<MODE>& expo) = default;
      Expo& operator=(const Expo<MODE>& expo) = default;
      Expo(Expo<MODE>&& expo) = default;
      Expo& operator=(Expo<MODE>&& expo) = default;
      ~Expo() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover
       */
      float GetProgressValue(const float& timeRatio) const;

      /*Each direction is the family's own algebra on a different argument, so only the
        argument changes between them. Checked here rather than left to the chain below,
        whose final else would quietly build any other value as In.*/
      static_assert(MODE == Ease::In || MODE == Ease::Out || MODE == Ease::InOut,
        "Ease::Mode must name one of the catalogue's three directions");
    };


    template<Ease::Mode MODE>
    inline Expo<MODE>::Expo(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Expo<MODE>>(start, end, duration, handler, total)
    {
    }

    template<Ease::Mode MODE>
    inline float Expo<MODE>::GetProgressValue(const float& timeRatio) const
    {
      /*This family is flat at both ends: its rate of change is zero at the start and at
        the end, which is the whole point of choosing it over a plain power. The raw
        formula leaves both ends at a rounded 0.0009 and 0.999, so the two ends are
        returned directly instead.*/
      if constexpr(MODE == Ease::Out)
      {
        if(timeRatio == 1.0f)
          return 1.0f;
        return 1.0f - std::pow(2.0f, -10.0f * timeRatio);
      }
      else if constexpr(MODE == Ease::InOut)
      {
        if(timeRatio == 0.0f)
          return 0.0f;
        if(timeRatio == 1.0f)
          return 1.0f;
        return timeRatio < 0.5f ?
          std::pow(2.0f, 20.0f * timeRatio - 10.0f) / 2.0f :
          (2.0f - std::pow(2.0f, -20.0f * timeRatio + 10.0f)) / 2.0f;
      }
      else
      {
        if(timeRatio == 0.0f)
          return 0.0f;
        return std::pow(2.0f, 10.0f * timeRatio - 10.0f);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_EXPO_H_