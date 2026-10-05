#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_BOUNCE_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_BOUNCE_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /*!< The shape of the catalogue's bounce: four parabolic segments of falling length, one
      per rebound, which is why the value reverses direction three times on its way in.*/
    static const float BOUNCE_SCALE = 7.5625f;
    static const float BOUNCE_DIVISOR = 2.75f;

    /**
     * @brief The bounce easing, a value that hits its end and rebounds off it
     *
     * @note A CRTP transition: the base template parameter is the derived class itself, so
     * Transition calls GetProgressValue on the derived object without a virtual call.
     * The direction is given at construction, so the three curves of this family are one
     * class rather than three that differ only in their body.
     */
    class EXTENDAPI Bounce: public Transition<Bounce>
    {
    public:
      /**
       * @brief Build a bounce transition that eases in the given direction
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param mode the direction the curve spends its shape at
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Bounce(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Bounce(const Bounce& bounce) = default;
      Bounce& operator=(const Bounce& bounce) = default;
      Bounce(Bounce&& bounce) = default;
      Bounce& operator=(Bounce&& bounce) = default;
      ~Bounce() = default;

      /**
       * @brief Map how much of the duration has passed to the fraction of the value made
       *
       * @param timeRatio the elapsed time over the duration, from 0 to 1
       * @return float the fraction of the distance to cover
       */
      float GetProgressValue(const float& timeRatio) const;

    protected:
      Ease::Mode m_mode; /*!< The direction this transition eases in*/

    private:
      /**
       * @brief The four parabolic segments the family is built from, run backwards
       *
       * @param timeRatio the elapsed fraction, which for the Out direction is the progress
       * already made
       * @return float the fraction of the distance covered
       */
      float BounceOut(const float& timeRatio) const;
    };


    inline Bounce::Bounce(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      const Ease::Mode& mode, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Bounce>(start, end, duration, handler, total), m_mode(mode)
    {
    }

    inline float Bounce::BounceOut(const float& timeRatio) const
    {
      /*Each segment is the same parabola shifted right and up, so the value it reaches at
        the far end of the segment is the height the next one starts falling from.*/
      if(timeRatio < 1.0f / BOUNCE_DIVISOR)
        return BOUNCE_SCALE * timeRatio * timeRatio;

      if(timeRatio < 2.0f / BOUNCE_DIVISOR)
      {
        const float shifted = timeRatio - 1.5f / BOUNCE_DIVISOR;
        return BOUNCE_SCALE * shifted * shifted + 0.75f;
      }

      if(timeRatio < 2.5f / BOUNCE_DIVISOR)
      {
        const float shifted = timeRatio - 2.25f / BOUNCE_DIVISOR;
        return BOUNCE_SCALE * shifted * shifted + 0.9375f;
      }

      const float shifted = timeRatio - 2.625f / BOUNCE_DIVISOR;
      return BOUNCE_SCALE * shifted * shifted + 0.984375f;
    }

    inline float Bounce::GetProgressValue(const float& timeRatio) const
    {
      switch(m_mode)
      {
        case Ease::Out:
          return BounceOut(timeRatio);
        case Ease::InOut:
          /*A bounce out over the first half, then the same bounce run backwards.*/
          return timeRatio < 0.5f ?
            (1.0f - BounceOut(1.0f - 2.0f * timeRatio)) / 2.0f :
            (1.0f + BounceOut(2.0f * timeRatio - 1.0f)) / 2.0f;
        case Ease::In:
        default:
          return 1.0f - BounceOut(1.0f - timeRatio);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_BOUNCE_H_