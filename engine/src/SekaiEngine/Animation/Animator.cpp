#include "SekaiEngine/Animation/Animator.h"

#include <utility>

namespace SekaiEngine {
  namespace Animation {
    Animator::Animator()
      :m_running(), m_pending(), m_discarded(), m_advancing(false), m_onTransitionEnd()
    {
    }

    void Animator::Play(std::unique_ptr<Node> animation)
    {
      /*The observer goes on before Start, so a transition of no length reports at
        hand-over, and a leaf anywhere under this node reports through the manager's one
        handler. The lambda reads the manager's handler each time it is called, so setting
        it again after this is felt.*/
      animation->SetFinishObserver([this](const Node& node)
      {
        if(m_onTransitionEnd)
          m_onTransitionEnd(node);
      });

      /*Started here rather than on the first advance: the handler receives the value the
        animation begins at at the moment the game asks for it, not a frame later. An
        animation of no duration reports its end value right away and is released on the
        first advance that reaches it, without ever having been advanced.*/
      animation->Start();
      m_pending.push_back(std::move(animation));
    }

    void Animator::Update(const SekaiEngine::Timestep& elipse)
    {
      /*Admitted at the top, never in the middle of a walk. An animation handed over before
        this advance began — from an event handler, say — has not been advanced yet, so it
        is advanced by this one; one handed over while this advance is running waits until
        the advance after it, because that is the first one to begin after it was handed
        over.*/
      for(std::size_t waiting = 0; waiting < m_pending.size(); waiting++)
        m_running.push_back(std::move(m_pending[waiting]));
      m_pending.clear();

      m_advancing = true;
      std::size_t running = 0;
      while(running < m_running.size())
      {
        m_running[running]->Update(elipse);

        /*A leaf reports as it finishes, from inside that Update, so a handler may have
          cleared everything this manager held while the Update was running. Clear defers
          the release to the end of this advance, so the animation whose Update just
          returned is still alive here; the list is read again before it is touched, and
          nothing discarded is advanced afterwards.*/
        if(running >= m_running.size())
          break;

        if(m_running[running]->IsFinish())
        {
          /*Released silently: the leaf already reported through its observer, and the
            manager never reports a container. The observer is dropped first, so nothing
            under the released tree still holds a handler pointing at this manager.*/
          m_running[running]->SetFinishObserver(nullptr);
          m_running.erase(m_running.begin() + running);
        }
        else
          running++;
      }
      m_advancing = false;

      /*Whatever a mid-advance Clear set aside is released here, once no Update is running
        on it.*/
      m_discarded.clear();
    }

    void Animator::Clear()
    {
      if(m_advancing)
      {
        /*Kept alive until the advance unwinds: the walk may be executing one of these
          nodes right now, and destroying it here would pull the object out from under the
          call. They are released when the advance ends.*/
        for(std::size_t held = 0; held < m_running.size(); held++)
          m_discarded.push_back(std::move(m_running[held]));
        m_running.clear();
        m_pending.clear();
        return;
      }

      /*Both lists: the ones being advanced and the ones waiting their first advance, since
        a game asking to discard everything means exactly that.*/
      m_running.clear();
      m_pending.clear();
    }

    void Animator::OnTransitionEnd(std::function<void(const Node&)> handler)
    {
      m_onTransitionEnd = std::move(handler);
    }

    std::size_t Animator::Count() const
    {
      return m_running.size() + m_pending.size();
    }
  } //namespace Animation
} //namespace SekaiEngine
