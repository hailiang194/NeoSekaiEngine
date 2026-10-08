#ifndef SEKAI_ENGINE_ANIMATION_SEQUENCE_H_
#define SEKAI_ENGINE_ANIMATION_SEQUENCE_H_

#include <memory>
#include <utility>
#include <vector>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Animation/Node.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief Children that run one after another
     *
     * @note Exactly one child is advanced at a time, so nothing of the second child is
     * reported before the first has reported its final value. Each child carries the two
     * values it was given: a sequence does not hand the value it finished on to the child
     * after it, so what a child animates does not depend on where it sits.
     */
    class EXTENDAPI Sequence: public Node
    {
    public:
      Sequence();
      Sequence(const Sequence& sequence) = delete;
      Sequence& operator=(const Sequence& sequence) = delete;
      Sequence(Sequence&& sequence) = default;
      Sequence& operator=(Sequence&& sequence) = default;
      ~Sequence() = default;

      /**
       * @brief Take ownership of one more child, to run after the ones already added
       *
       * @param child the animation this sequence now owns
       */
      void Add(std::unique_ptr<Node> child);

      void Start() override;
      void Update(const SekaiEngine::Timestep& elipse) override;
      const bool IsFinish() const override;
      void Reverse() override;

      /**
       * @brief Set what is told when a leaf this sequence holds finishes, on every child
       *
       * @param observer passed down to each child, present and future
       */
      void SetFinishObserver(std::function<void(const Node&)> observer) override;

    protected:
      std::vector<std::unique_ptr<Node>> m_children; /*!< The animations, in the order they run*/
      std::size_t m_index; /*!< The child running now, or past the end once finished*/

    private:
      /**
       * @brief Start the child after the current one, once the current one has finished
       *
       * @note One hand-over per advance: the new child reports its start value in the frame
       * the previous one ended in, and takes its first step on the next frame. Nothing is
       * given the time that was spent finishing the child before it.
       *
       * ponytail: one boundary crossing per advance — elapsed beyond the current child is
       * dropped. Expose a consumed-time query on the base if frame spikes ever make a chain
       * visibly lag.
       */
      void HandOver();
    };

    inline Sequence::Sequence()
      :m_children(), m_index(0)
    {
    }

    inline void Sequence::Add(std::unique_ptr<Node> child)
    {
      /*Handed the observer the sequence already holds, so a child added after the manager
        set one still reports through it.*/
      child->SetFinishObserver(m_onFinish);
      m_children.push_back(std::move(child));
    }

    inline void Sequence::Start()
    {
      /*Whatever had run before, a sequence that is started again begins at its first child.*/
      m_index = 0;
      if(!IsFinish())
        m_children[0]->Start();
    }

    inline void Sequence::Update(const SekaiEngine::Timestep& elipse)
    {
      if(IsFinish())
        return;

      m_children[m_index]->Update(elipse);
      HandOver();
    }

    inline const bool Sequence::IsFinish() const
    {
      /*An empty sequence has nothing left to run, which is what a finished one reports.*/
      return m_index >= m_children.size();
    }

    inline void Sequence::Reverse()
    {
      /*Every leaf goes the other way and the order does not move: reversing twice is the
        identity, because each child's own reversal is one.*/
      for(std::size_t child = 0; child < m_children.size(); child++)
        m_children[child]->Reverse();
    }

    inline void Sequence::SetFinishObserver(std::function<void(const Node&)> observer)
    {
      Node::SetFinishObserver(observer);
      for(std::size_t child = 0; child < m_children.size(); child++)
        m_children[child]->SetFinishObserver(observer);
    }

    inline void Sequence::HandOver()
    {
      if(!m_children[m_index]->IsFinish())
        return;

      m_index++;
      if(!IsFinish())
        m_children[m_index]->Start();
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_SEQUENCE_H_
