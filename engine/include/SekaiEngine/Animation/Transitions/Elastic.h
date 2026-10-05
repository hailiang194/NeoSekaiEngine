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
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Elastic: public Transition<Elastic>
    {
    public:
      /**
       * @brief Build an elastic transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Elastic(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Elastic(const Elastic& elastic) = default;
      Elastic& operator=(const Elastic& elastic) = default;
      Elastic(Elastic&& elastic) = default;
      Elastic& operator=(Elastic&& elastic) = default;
      ~Elastic() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover, which for this family swings
       * beyond both of its end values before settling
       */
      float GetProgressValue(const float& timeRatio) const;

    protected:
      Ease::Mode m_mode; /*!< The direction this transition eases in*/
    };


    inline Elastic::Elastic(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Elastic>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Elastic::GetProgressValue(const float& timeRatio) const
    {
      /*This is the other family the catalogue defines to overshoot: the value swings past
        both of its ends, then the oscillation damps out and it settles on the end value.
        Both ends are returned directly, because the sine of the raw expression is not
        exactly its limit there and a transition must still report its end exactly.*/
      switch(m_mode)
      {
        case Ease::Out:
          if(timeRatio == 0.0f)
            return 0.0f;
          if(timeRatio == 1.0f)
            return 1.0f;
          return std::pow(2.0f, -10.0f * timeRatio) *
            std::sin((timeRatio * 10.0f - 0.75f) * ELASTIC_PERIOD) + 1.0f;
        case Ease::InOut:
          if(timeRatio == 0.0f)
            return 0.0f;
          if(timeRatio == 1.0f)
            return 1.0f;
          return timeRatio < 0.5f ?
            -(std::pow(2.0f, 20.0f * timeRatio - 10.0f) *
              std::sin((20.0f * timeRatio - 11.125f) * ELASTIC_INOUT_PERIOD)) / 2.0f :
            (std::pow(2.0f, -20.0f * timeRatio + 10.0f) *
              std::sin((20.0f * timeRatio - 11.125f) * ELASTIC_INOUT_PERIOD)) / 2.0f + 1.0f;
        case Ease::In:
        default:
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