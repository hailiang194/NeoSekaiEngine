#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_SINE_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_SINE_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /*!< Half of pi, the angle the In and Out forms sweep through*/
    static const float HALF_PI = 1.5707963267948966f;

    /**
     * @brief The sinusoidal easing, a quarter of a cosine
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
    class EXTENDAPI Sine: public Transition<Sine<MODE>>
    {
    public:
      /**
       * @brief Build a sinusoidal transition that eases in the direction its type names
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Sine(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Sine(const Sine<MODE>& sine) = default;
      Sine& operator=(const Sine<MODE>& sine) = default;
      Sine(Sine<MODE>&& sine) = default;
      Sine& operator=(Sine<MODE>&& sine) = default;
      ~Sine() = default;

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
    inline Sine<MODE>::Sine(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Sine<MODE>>(start, end, duration, handler, total)
    {
    }

    template<Ease::Mode MODE>
    inline float Sine<MODE>::GetProgressValue(const float& timeRatio) const
    {
      /*The shift at the start is what makes this family gentler than a power: the value
        leaves its start with no rate of change at all.*/
      if constexpr(MODE == Ease::Out)
      {
        return std::sin(timeRatio * HALF_PI);
      }
      else if constexpr(MODE == Ease::InOut)
      {
        /*The full half-cosine over the whole duration, halved so it spans 0 to 1.
          Written with the subtraction the other way round so the start of the duration
          is a positive zero rather than the negative one the catalogue's form yields.*/
        return (1.0f - std::cos(2.0f * HALF_PI * timeRatio)) / 2.0f;
      }
      else
      {
        return 1.0f - std::cos(timeRatio * HALF_PI);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_SINE_H_