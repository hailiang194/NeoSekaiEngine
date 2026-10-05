#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUINT_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUINT_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The quintic easing, the fifth power of the elapsed fraction
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Quint: public Transition<Quint>
    {
    public:
      /**
       * @brief Build a quintic transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Quint(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Quint(const Quint& quint) = default;
      Quint& operator=(const Quint& quint) = default;
      Quint(Quint&& quint) = default;
      Quint& operator=(Quint&& quint) = default;
      ~Quint() = default;

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


    inline Quint::Quint(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Quint>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Quint::GetProgressValue(const float& timeRatio) const
    {
      switch(m_mode)
      {
        case Ease::Out:
          return 1.0f - std::pow(1.0f - timeRatio, 5.0f);
        case Ease::InOut:
          return timeRatio < 0.5f ?
            16.0f * std::pow(timeRatio, 5.0f) : 1.0f - 16.0f * std::pow(1.0f - timeRatio, 5.0f);
        case Ease::In:
        default:
          return std::pow(timeRatio, 5.0f);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUINT_H_