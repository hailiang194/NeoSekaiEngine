#ifndef SEKAI_ENGINE_ANIMATION_TRANSITION_H_
#define SEKAI_ENGINE_ANIMATION_TRANSITION_H_

#include <algorithm>
#include <functional>
#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"


namespace SekaiEngine {
  namespace Animation {

    using OnUpdateHandler = std::function<void(const float&)>;
    template<typename AnimationFunction>
    class EXTENDAPI Transition
    {
    public:
      Transition(const float& start, const float& end, const SekaiEngine::Timestep& duration, OnUpdateHandler handler, const SekaiEngine::Timestep& total = SekaiEngine::Timestep());
      Transition(const Transition& animation) = default;
      Transition& operator=(const Transition& animation) = default;
      Transition(Transition&& animation) = default;
      Transition& operator=(Transition&& animation) = default;
      ~Transition() = default;

      void Update(const SekaiEngine::Timestep& elipse);
      void Start();

      const bool IsFinish() const;

    protected:
      SekaiEngine::Timestep m_duration; /*!< The duration for the animation*/
      SekaiEngine::Timestep m_total; /*!< The total time the animation has run*/
      float m_start; /*!< Position when the transition start*/
      float m_end; /*!< Position when the transition finish*/
      OnUpdateHandler m_OnUpdate; /*!< Function called when the transition is updated*/
    };


    template<typename AnimationFunction>
    inline Transition<AnimationFunction>::Transition(const float& start, const float& end, const SekaiEngine::Timestep& duration, OnUpdateHandler handler, const SekaiEngine::Timestep& total)
      :m_duration(duration), m_total(total), m_start(start), m_end(end), m_OnUpdate(handler)
    {

    }

    template<typename AnimationFunction>
    inline void Transition<AnimationFunction>::Update(const SekaiEngine::Timestep& elipse)
    {
      if(IsFinish())
        return;

       /*Both the elapsed time and the duration are seconds, so the elapsed time is added in
         seconds and the ratio handed to the transition function is unitless.*/
      m_total = SekaiEngine::Timestep(std::min(float(m_total) + float(elipse), float(m_duration)));
      AnimationFunction* delived = static_cast<AnimationFunction*>(this);
      /*A zero-length transition has no ratio to divide by, and it has nowhere to go anyway:
        it is over the moment it starts.*/
      float progress = (m_duration > 0.0f) ?
        delived->GetProgressValue(float(m_total) / float(m_duration)) : 1.0f;
      float position = m_start + (m_end - m_start) * progress;
      m_OnUpdate(position);
    }
    
    template<typename AnimationFunction>
    inline const bool Transition<AnimationFunction>::IsFinish() const
    {
       /*Not ==: the elapsed time is an accumulation of floats, so it is compared as an
         inequality. It is clamped to the duration on every update, so it cannot overshoot.*/
      return m_total >= m_duration;
    }

    template<typename AnimationFunction>
    inline void Transition<AnimationFunction>::Start()
    {
      /*Rewound, so a finished transition can be played again from its start value.*/
      m_total = SekaiEngine::Timestep();
      /*A transition with no duration is already over the moment it starts, so there is no
        progress left to step and the end value is reported straight away.*/
       if(m_duration > 0.0f)
         Update(SekaiEngine::Timestep());
       else
         m_OnUpdate(m_end);
    }
  
  } //namespace Animation

} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_TRANSITION_H_
