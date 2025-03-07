#include "event.hpp"
#include "exception.hpp"
#include "raycast/behaviour_system.hpp"
#include "utils.hpp"


using std::set;


//===========================================
// BehaviourSystem::update
//===========================================
void BehaviourSystem::update() {
  for (auto it = m_components.begin(); it != m_components.end(); ++it) {
    it->second->update();
  }
}

//===========================================
// BehaviourSystem::handleEvent
//===========================================
void BehaviourSystem::handleEvent(const GameEvent& event, const set<entityId_t>& entities) {
  for (auto it = m_components.begin(); it != m_components.end(); ++it) {
    if (entities.count(it->first)) {
      CBehaviour& c = *it->second;
      c.handleTargetedEvent(event);
    }
  }
}

//===========================================
// BehaviourSystem::handleEvent
//===========================================
void BehaviourSystem::handleEvent(const GameEvent& event) {
  for (auto it = m_components.begin(); it != m_components.end(); ++it) {
    CBehaviour& c = *it->second;
    c.handleBroadcastedEvent(event);
  }
}

//===========================================
// BehaviourSystem::hasComponent
//===========================================
bool BehaviourSystem::hasComponent(entityId_t entityId) const {
  return m_components.find(entityId) != m_components.end();
}

//===========================================
// BehaviourSystem::getComponent
//===========================================
CBehaviour& BehaviourSystem::getComponent(entityId_t entityId) const {
  return *m_components.at(entityId);
}

//===========================================
// BehaviourSystem::addComponent
//===========================================
void BehaviourSystem::addComponent(pComponent_t component) {
  if (component->kind() != ComponentKind::C_BEHAVIOUR) {
    EXCEPTION("Component is not of type CBehaviour");
  }

  CBehaviour* p = dynamic_cast<CBehaviour*>(component.release());
  m_components.insert(std::make_pair(p->entityId(), pCBehaviour_t(p)));
}

//===========================================
// BehaviourSystem::removeEntity
//===========================================
void BehaviourSystem::removeEntity(entityId_t entityId) {
  m_components.erase(entityId);
}
