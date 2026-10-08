#ifndef SEKAI_ENGINE_ANIMATION_REPEAT_H_
#define SEKAI_ENGINE_ANIMATION_REPEAT_H_

#include <limits>
#include <memory>
#include <utility>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Animation/Node.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief One child run a count of times, or forever
     *
     * @note The count is a count of runs in both orders: replay runs the child from its
     * start value to its end value once per unit, and ping-pong alternates between the two
     * so that an even count ends where it began. Because a run is one unit either way, the
     * finish test is a single comparison and a count of forever is just a very large one.
     */
    class EXTENDAPI Repeat: public Node
    {
    public:
      /**
       * @brief The way the runs follow one another
       *
       * @note Every run travels the same way in Replay, and they alternate in PingPong.
       */
      enum Order
      {
        Replay, /*!< Every run travels from the start value to the end value*/
        PingPong /*!< Runs alternate between travelling to the end value and back*/
      };

      /*!< A count no run reaches: the repeat never reports itself finished.*/
      static constexpr int Forever = std::numeric_limits<int>::max();

      Repeat(std::unique_ptr<Node> child, const int& count, const Order& order);
      Repeat(const Repeat& repeat) = delete;
      Repeat& operator=(const Repeat& repeat) = delete;
      Repeat(Repeat&& repeat) = default;
      Repeat& operator=(Repeat&& repeat) = default;
      ~Repeat() = default;

      void Start() override;
      void Update(const SekaiEngine::Timestep& elipse) override;
      const bool IsFinish() const override;
      void Reverse() override;

      /**
       * @brief Set what is told when the child finishes, on the child and every run after
       *
       * @param observer passed down to the child, now and each time it is restarted
       */
      void SetFinishObserver(std::function<void(const Node&)> observer) override;

    protected:
      std::unique_ptr<Node> m_child; /*!< The animation this repeat runs*/
      int m_count; /*!< How many runs the repeat performs before it finishes*/
      Order m_order; /*!< Whether the runs all travel one way or alternate*/
      int m_cycle; /*!< How many runs have completed*/
      bool m_reversed; /*!< Whether the repeat itself has been turned around*/
      bool m_forward; /*!< Whether the child is oriented the way it was built*/

    private:
      /**
       * @brief Put the child the way this run travels, and begin it
       *
       * @note The orientation is asked for rather than remembered across runs, so a repeat
       * started again always begins travelling the way it was built, however the previous
       * count of ping-pong runs happened to end.
       */
      void BeginRun();
    };

    inline Repeat::Repeat(std::unique_ptr<Node> child, const int& count, const Order& order)
      :m_child(std::move(child)), m_count(count), m_order(order), m_cycle(0),
      m_reversed(false), m_forward(true)
    {
    }

    inline void Repeat::Start()
    {
      /*A repeat that is started again performs its count from the first run.*/
      m_cycle = 0;
      BeginRun();
    }

    inline void Repeat::Update(const SekaiEngine::Timestep& elipse)
    {
      if(IsFinish())
        return;

      m_child->Update(elipse);

      if(!m_child->IsFinish())
        return;

      /*A run has ended. One boundary per advance: the run after it begins here, with its
        own start value reported in this same frame, and takes its first step next frame.*/
      m_cycle++;
      if(IsFinish())
        return;

      BeginRun();
    }

    inline const bool Repeat::IsFinish() const
    {
      /*No branch for forever: a count of Forever is simply one no run reaches. Not ==,
        because the child may have been left mid-run by a repeat that is still going.*/
      return m_cycle >= m_count;
    }

    inline void Repeat::Reverse()
    {
      /*The count and the order do not move; only the leaves do, and the repeat's own
        sense of which way is forward turns with them so that the runs after this one still
        alternate correctly. Reversing twice restores it, because each step is one flip.*/
      m_reversed = !m_reversed;
      m_child->Reverse();
      m_forward = !m_forward;
    }

    inline void Repeat::BeginRun()
    {
      bool forwards = !m_reversed;
      if(m_order == PingPong && (m_cycle % 2) != 0)
        forwards = !forwards;

      if(forwards != m_forward)
      {
        m_child->Reverse();
        m_forward = forwards;
      }

      /*Handed the observer the repeat holds, so the child reports at the end of this run,
        including under a count of forever.*/
      m_child->SetFinishObserver(m_onFinish);
      m_child->Start();
    }

    inline void Repeat::SetFinishObserver(std::function<void(const Node&)> observer)
    {
      Node::SetFinishObserver(observer);
      m_child->SetFinishObserver(observer);
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_REPEAT_H_
