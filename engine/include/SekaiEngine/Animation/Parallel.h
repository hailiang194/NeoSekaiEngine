#ifndef SEKAI_ENGINE_ANIMATION_PARALLEL_H_
#define SEKAI_ENGINE_ANIMATION_PARALLEL_H_

#include <memory>
#include <utility>
#include <vector>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Animation/Node.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief Children that all run at the same time
     *
     * @note Every child is advanced by the same elapsed time on every advance, and the group
     * is finished only once all of them are, so a group given children of different lengths
     * outlives its shortest member.
     */
    class EXTENDAPI Parallel: public Node
    {
    public:
      Parallel();
      Parallel(const Parallel& parallel) = delete;
      Parallel& operator=(const Parallel& parallel) = delete;
      Parallel(Parallel&& parallel) = default;
      Parallel& operator=(Parallel&& parallel) = default;
      ~Parallel() = default;

      /**
       * @brief Take ownership of one more child, to run alongside the ones already added
       *
       * @param child the animation this group now owns
       */
      void Add(std::unique_ptr<Node> child);

      void Start() override;
      void Update(const SekaiEngine::Timestep& elipse) override;
      const bool IsFinish() const override;
      void Reverse() override;

      /**
       * @brief Set what is told when a leaf this group holds finishes, on every child
       *
       * @param observer passed down to each child, present and future
       */
      void SetFinishObserver(std::function<void(const Node&)> observer) override;

    protected:
      std::vector<std::unique_ptr<Node>> m_children; /*!< The animations, all running together*/
    };

    inline Parallel::Parallel()
      :m_children()
    {
    }

    inline void Parallel::Add(std::unique_ptr<Node> child)
    {
      /*Handed the observer the group already holds, so a child added after the manager set
        one still reports through it.*/
      child->SetFinishObserver(m_onFinish);
      m_children.push_back(std::move(child));
    }

    inline void Parallel::Start()
    {
      /*All of them, because the group means them to be simultaneous: starting only the
        first would report every child's start value in the wrong order across one frame.*/
      for(std::size_t child = 0; child < m_children.size(); child++)
        m_children[child]->Start();
    }

    inline void Parallel::Update(const SekaiEngine::Timestep& elipse)
    {
      for(std::size_t child = 0; child < m_children.size(); child++)
        m_children[child]->Update(elipse);
    }

    inline const bool Parallel::IsFinish() const
    {
      /*Finished only once every child is: an empty group has nothing left to run.*/
      for(std::size_t child = 0; child < m_children.size(); child++)
      {
        if(!m_children[child]->IsFinish())
          return false;
      }

      return true;
    }

    inline void Parallel::Reverse()
    {
      /*Every member goes the other way. The group's own completion does not move, because
        each member still finishes after the duration it was given.*/
      for(std::size_t child = 0; child < m_children.size(); child++)
        m_children[child]->Reverse();
    }

    inline void Parallel::SetFinishObserver(std::function<void(const Node&)> observer)
    {
      Node::SetFinishObserver(observer);
      for(std::size_t child = 0; child < m_children.size(); child++)
        m_children[child]->SetFinishObserver(observer);
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_PARALLEL_H_
