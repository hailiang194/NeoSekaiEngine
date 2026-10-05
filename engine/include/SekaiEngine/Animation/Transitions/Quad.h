#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUAD_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUAD_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The quadratic easing, the square of the elapsed fraction
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Quad: public Transition<Quad>
    {
    public:
      /**
       * @brief Build a quadratic transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Quad(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Quad(const Quad& quad) = default;
      Quad& operator=(const Quad& quad) = default;
      Quad(Quad&& quad) = default;
      Quad& operator=(Quad&& quad) = default;
      ~Quad() = default;

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


    inline Quad::Quad(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Quad>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Quad::GetProgressValue(const float& timeRatio) const
    {
      /*Each direction is the family's own algebra on a different argument, so the shape
        is written once and only the argument changes.*/
      switch(m_mode)
      {
        case Ease::Out:
          /*Squares the distance still to travel, so the value races away from its end.*/
          return 1.0f - (1.0f - timeRatio) * (1.0f - timeRatio);
        case Ease::InOut:
          return timeRatio < 0.5f ?
            2.0f * timeRatio * timeRatio : 1.0f - 2.0f * (1.0f - timeRatio) * (1.0f - timeRatio);
        case Ease::In:
        default:
          return timeRatio * timeRatio;
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUAD_H_