#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_ELASTIC_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_ELASTIC_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /*!< How fast the catalogue's Elastic curve oscillates. The In and Out forms sweep two
      thirds of a turn per unit of the exponential's own cycle, and the InOut form sweeps
      rather less, so each has its own.*/
    static const float ELASTIC_PERIOD = 2.0943951023931953f; /*!< 2 * pi / 3*/
    static const float ELASTIC_INOUT_PERIOD = 1.3962634015954636f; /*!< 2 * pi / 4.5*/

    /**
     * @brief The elastic easing, a curve that oscillates past its end before settling
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
    class EXTENDAPI Elastic: public Transition<Elastic<MODE>>
    {
    public:
      /**
       * @brief Build an elastic transition that eases in the direction its type names
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Elastic(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Elastic(const Elastic<MODE>& elastic) = default;
      Elastic& operator=(const Elastic<MODE>& elastic) = default;
      Elastic(Elastic<MODE>&& elastic) = default;
      Elastic& operator=(Elastic<MODE>&& elastic) = default;
      ~Elastic() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover, which for this family swings
       * beyond both of its end values before settling
       */
      float GetProgressValue(const float& timeRatio) const;

      /*Each direction is the family's own algebra on a different argument, so only the
        argument changes between them. Checked here rather than left to the chain below,
        whose final else would quietly build any other value as In.*/
      static_assert(MODE == Ease::In || MODE == Ease::Out || MODE == Ease::InOut,
        "Ease::Mode must name one of the catalogue's three directions");
    };


    template<Ease::Mode MODE>
    inline Elastic<MODE>::Elastic(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Elastic<MODE>>(start, end, duration, handler, total)
    {
    }

    template<Ease::Mode MODE>
    inline float Elastic<MODE>::GetProgressValue(const float& timeRatio) const
    {
      /*This is the other family the catalogue defines to overshoot: the value swings past
        both of its ends, then the oscillation damps out and it settles on the end value.
        Both ends are returned directly, because the sine of the raw expression is not
        exactly its limit there and a transition must still report its end exactly.*/
      if constexpr(MODE == Ease::Out)
      {
        if(timeRatio == 0.0f)
          return 0.0f;
        if(timeRatio == 1.0f)
          return 1.0f;
        return std::pow(2.0f, -10.0f * timeRatio) *
          std::sin((timeRatio * 10.0f - 0.75f) * ELASTIC_PERIOD) + 1.0f;
      }
      else if constexpr(MODE == Ease::InOut)
      {
        if(timeRatio == 0.0f)
          return 0.0f;
        if(timeRatio == 1.0f)
          return 1.0f;
        return timeRatio < 0.5f ?
          -(std::pow(2.0f, 20.0f * timeRatio - 10.0f) *
            std::sin((20.0f * timeRatio - 11.125f) * ELASTIC_INOUT_PERIOD)) / 2.0f :
          (std::pow(2.0f, -20.0f * timeRatio + 10.0f) *
            std::sin((20.0f * timeRatio - 11.125f) * ELASTIC_INOUT_PERIOD)) / 2.0f + 1.0f;
      }
      else
      {
        if(timeRatio == 0.0f)
          return 0.0f;
        if(timeRatio == 1.0f)
          return 1.0f;
        return -std::pow(2.0f, 10.0f * timeRatio - 10.0f) *
          std::sin((timeRatio * 10.0f - 10.75f) * ELASTIC_PERIOD);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_ELASTIC_H_