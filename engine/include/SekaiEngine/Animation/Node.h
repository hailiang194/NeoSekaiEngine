#ifndef SEKAI_ENGINE_ANIMATION_NODE_H_
#define SEKAI_ENGINE_ANIMATION_NODE_H_

#include <functional>
#include <utility>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief The one interface every animation answers to
     *
     * @note A container holds nodes, not curve types, so it never has to know which of the
     * Transition variants it is holding. A leaf is a Transition-derived class and reaches
     * here through one base class; a container is a Node of its own that owns other nodes.
     *
     * @note Every operation is asked for in elapsed seconds rather than read from a clock,
     * so a whole tree can be stepped by a caller that has no window and no frame loop.
     */
    class EXTENDAPI Node
    {
    public:
      Node() = default;
      virtual ~Node() = default;
      Node(const Node& node) = default;
      Node& operator=(const Node& node) = default;
      Node(Node&& node) = default;
      Node& operator=(Node&& node) = default;

      /**
       * @brief Begin the animation from wherever it currently is, reporting its first value
       *
       */
      virtual void Start() = 0;

      /**
       * @brief Advance the animation by an elapsed time and report where it has reached
       *
       * @param elipse elapsed time in seconds
       */
      virtual void Update(const SekaiEngine::Timestep& elipse) = 0;

      /**
       * @brief Whether the animation has run out of work
       *
       * @return const bool true when the animation has nothing left to run
       */
      virtual const bool IsFinish() const = 0;

      /**
       * @brief Send the animation the other way without rebuilding it
       *
       * @note A leaf exchanges its two values; a container reverses every leaf it holds and
       * changes no structure, so reversing twice is an identity.
       */
      virtual void Reverse() = 0;

      /**
       * @brief Set what is told when a leaf transition this node holds finishes
       *
       * @param observer called with the finished leaf, every time it finishes; an empty
       * observer means nothing is told
       *
       * @note A leaf reports through it and an empty observer is silent. A container
       * forwards it to each child, so a leaf at any depth reports through the one observer
       * a game sets on the manager. A container never calls it itself.
       */
      virtual void SetFinishObserver(std::function<void(const Node&)> observer);

    protected:
      /**
       * @brief Tell the observer, if one is set, that this node has finished
       *
       * @note Called by a leaf when a run crosses into finished. A container does not call
       * it, because a container is not a transition and never reports.
       */
      void NotifyFinish();

      std::function<void(const Node&)> m_onFinish; /*!< What is told when a leaf finishes*/

    };

    inline void Node::SetFinishObserver(std::function<void(const Node&)> observer)
    {
      m_onFinish = std::move(observer);
    }

    inline void Node::NotifyFinish()
    {
      if(m_onFinish)
        m_onFinish(*this);
    }
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_NODE_H_
