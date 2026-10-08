#ifndef SEKAI_ENGINE_ANIMATION_ANIMATOR_H_
#define SEKAI_ENGINE_ANIMATION_ANIMATOR_H_

#include <functional>
#include <memory>
#include <vector>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Animation/Node.h"

namespace SekaiEngine {
  namespace Animation {
    /**
     * @brief Owns every animation a game hands to the engine, advances them, and lets go of
     * the ones that have finished
     *
     * @note A game never steps an animation itself. It builds one, hands it over, and the
     * manager starts it there and then, advances it on every frame, and drops it once it
     * reports itself finished.
     *
     * @note Two lists, because an animation may be handed over from inside another
     * animation's own handler, which runs while the running list is being walked. A new one
     * waits in the pending list until the walk is over, so nothing is ever added to or
     * removed from the list under the walk.
     *
     * @note Advancing contacts no engine subsystem beyond the animations themselves: it
     * needs no window, no display, no graphics context and no frame loop.
     */
    class EXTENDAPI Animator
    {
    public:
      Animator();
      Animator(const Animator& animator) = delete;
      Animator& operator=(const Animator& animator) = delete;
      Animator(Animator&& animator) = default;
      Animator& operator=(Animator&& animator) = default;
      ~Animator() = default;

      /**
       * @brief Take ownership of an animation, starting it at once
       *
       * @param animation the animation the manager now owns
       *
       * @note Started here rather than on the first advance, so that the handler receives
       * the value the animation begins at at the moment the game asks for it. It is
       * advanced first by the first advance that begins after this call, never by one
       * already in progress.
       */
      void Play(std::unique_ptr<Node> animation);

      /**
       * @brief Advance everything owned by an elapsed time, and release what has finished
       *
       * @param elipse elapsed time in seconds
       */
      void Update(const SekaiEngine::Timestep& elipse);

      /**
       * @brief Let go of everything held, finished or not
       *
       * @note Safe to call from inside an animation's own handler: the advance in progress
       * defers the release until it has unwound, and nothing discarded is advanced
       * afterwards.
       */
      void Clear();

      /**
       * @brief Set what is told when a transition this manager owns finishes
       *
       * @param handler called once with each finished leaf transition, every time it
       * finishes; an empty handler means nothing is told
       *
       * @note The manager reports every leaf, whether it holds the leaf directly or holds a
       * container that does, and reports no container. The leaf is passed while it is still
       * alive and the reference is valid only for the duration of the call.
       *
       * @note A leaf that is discarded rather than finished is not reported. A leaf that
       * runs again inside a repeat is reported again when it finishes again.
       */
      void OnTransitionEnd(std::function<void(const Node&)> handler);

      /**
       * @brief How many animations the manager is holding
       *
       * @return std::size_t the ones waiting to be advanced plus the ones being advanced
       */
      std::size_t Count() const;

    protected:
      std::vector<std::unique_ptr<Node>> m_running; /*!< The animations being advanced*/
      std::vector<std::unique_ptr<Node>> m_pending; /*!< The animations waiting their first advance*/
      std::vector<std::unique_ptr<Node>> m_discarded; /*!< What a mid-advance Clear deferred*/
      bool m_advancing; /*!< Whether an advance is running, so Clear can defer*/
      std::function<void(const Node&)> m_onTransitionEnd; /*!< What leaf finishes are reported to*/
    };
  } //namespace Animation
} //namespace SekaiEngine

#endif //!SEKAI_ENGINE_ANIMATION_ANIMATOR_H_
