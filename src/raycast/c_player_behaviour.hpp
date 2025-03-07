#ifndef __PROCALC_RAYCAST_C_PLAYER_BEHAVIOUR_HPP_
#define __PROCALC_RAYCAST_C_PLAYER_BEHAVIOUR_HPP_


#include <vector>
#include "raycast/behaviour_system.hpp"
#include "raycast/system_accessor.hpp"


class EntityManager;
class TimeService;

class CPlayerBehaviour : public CBehaviour, private SystemAccessor {
  public:
    CPlayerBehaviour(entityId_t entityId, EntityManager& entityManager, TimeService& timeService);

    virtual void update() override;
    virtual void handleBroadcastedEvent(const GameEvent&) override {}
    virtual void handleTargetedEvent(const GameEvent& e) override;

  private:
    EntityManager& m_entityManager;
    TimeService& m_timeService;

    void onEntityDamaged(const GameEvent& e_);
    void onEntityDestroyed();
    void onPlayerMove();
    void onAnimationFinished(const GameEvent& e_);
};


#endif
