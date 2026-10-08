#ifndef SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUART_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUART_H_

#include <cmath>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Animation/Transition.h"
#include "SekaiEngine/Animation/Transitions/Ease.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The quartic easing, the fourth power of the elapsed fraction
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
    class EXTENDAPI Quart: public Transition<Quart<MODE>>
    {
    public:
      /**
       * @brief Build a quartic transition that eases in the direction its type names
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Quart(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Quart(const Quart<MODE>& quart) = default;
      Quart& operator=(const Quart<MODE>& quart) = default;
      Quart(Quart<MODE>&& quart) = default;
      Quart& operator=(Quart<MODE>&& quart) = default;
      ~Quart() = default;

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
    inline Quart<MODE>::Quart(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Quart<MODE>>(start, end, duration, handler, total)
    {
    }

    template<Ease::Mode MODE>
    inline float Quart<MODE>::GetProgressValue(const float& timeRatio) const
    {
      if constexpr(MODE == Ease::Out)
      {
        return 1.0f - std::pow(1.0f - timeRatio, 4.0f);
      }
      else if constexpr(MODE == Ease::InOut)
      {
        return timeRatio < 0.5f ?
          8.0f * std::pow(timeRatio, 4.0f) : 1.0f - 8.0f * std::pow(1.0f - timeRatio, 4.0f);
      }
      else
      {
        return std::pow(timeRatio, 4.0f);
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_QUART_H_