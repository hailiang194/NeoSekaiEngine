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
     * The direction is a template parameter too, so the three curves of this family share
     * one body and an instantiation is left holding only the shape it was asked for.
     *
     * @tparam MODE the direction the shape is spent at, one of Ease::In, Ease::Out or
     * Ease::InOut
     */
    template<Ease::Mode MODE>
    class EXTENDAPI Circ: public Transition<Circ<MODE>>
    {
    public:
      /**
       * @brief Build a circular transition that eases in the direction its type names
       *
       * @param start the value the transition starts at
       * @param end the value the transition finishes at
       * @param duration the seconds the transition takes to cover the whole distance
       * @param handler the function called with the value on every step
       * @param total the time the transition has already run, for a transition being
       * resumed part way through
       */
      Circ(const float& start, const float& end, const SekaiEngine::Timestep& duration,
        OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());

      Circ(const Circ<MODE>& circ) = default;
      Circ& operator=(const Circ<MODE>& circ) = default;
      Circ(Circ<MODE>&& circ) = default;
      Circ& operator=(Circ<MODE>&& circ) = default;
      ~Circ() = default;

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
    inline Circ<MODE>::Circ(const float& start, const float& end, const SekaiEngine::Timestep& duration,
      OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :Transition<Circ<MODE>>(start, end, duration, handler, total)
    {
    }

    template<Ease::Mode MODE>
    inline float Circ<MODE>::GetProgressValue(const float& timeRatio) const
    {
      if constexpr(MODE == Ease::Out)
      {
        return std::sqrt(1.0f - std::pow(timeRatio - 1.0f, 2.0f));
      }
      else if constexpr(MODE == Ease::InOut)
      {
        /*Two quarter circles, each run over its own half of the duration.*/
        return timeRatio < 0.5f ?
          (1.0f - std::sqrt(1.0f - std::pow(2.0f * timeRatio, 2.0f))) / 2.0f :
          (std::sqrt(1.0f - std::pow(2.0f * timeRatio - 2.0f, 2.0f)) + 1.0f) / 2.0f;
      }
      else
      {
        return 1.0f - std::sqrt(1.0f - std::pow(timeRatio, 2.0f));
      }
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITIONS_CIRC_H_