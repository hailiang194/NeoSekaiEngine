#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_CIRC_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_CIRC_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The circular easing, a quarter circle
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Circ: public Transition<Circ>
    {
    public:
      /**
       * @brief Build a circular transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Circ(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Circ(const Circ& circ) = default;
      Circ& operator=(const Circ& circ) = default;
      Circ(Circ&& circ) = default;
      Circ& operator=(Circ&& circ) = default;
      ~Circ() = default;

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


    inline Circ::Circ(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Circ>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Circ::GetProgressValue(const float& timeRatio) const
    {
      switch(m_mode)
      {
        case Ease::Out:
          return std::sqrt(1.0f - std::pow(timeRatio - 1.0f, 2.0f));
        case Ease::InOut:
          /*Two quarter circles, each run over its own half of the duration.*/
          return timeRatio < 0.5f ?
            (1.0f - std::sqrt(1.0f - std::pow(2.0f * timeRatio, 2.0f))) / 2.0f :
            (std::sqrt(1.0f - std::pow(2.0f * timeRatio - 2.0f, 2.0f)) + 1.0f) / 2.0f;
        case Ease::In:
        default:
          return 1.0f - std::sqrt(1.0f - std::pow(timeRatio, 2.0f));
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_CIRC_H_