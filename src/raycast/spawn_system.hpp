#ifndef __PROCALC_RAYCAST_SPAWN_SYSTEM_HPP_
#define __PROCALC_RAYCAST_SPAWN_SYSTEM_HPP_


#include <memory>
#include <map>
#include "raycast/system.hpp"
#include "raycast/component.hpp"
#include "raycast/map_parser.hpp"


enum class CSpawnKind {
  SPAWNABLE,
  SPAWN_POINT
};

struct CSpawn : public Component {
  CSpawn(entityId_t entityId, CSpawnKind kind)
    : Component(entityId, ComponentKind::C_SPAWN),
      kind(kind) {}

  CSpawnKind kind;

  virtual ~CSpawn() = 0;
};

struct CSpawnable : public CSpawn {
  CSpawnable(entityId_t entityId, entityId_t spawnPoint, const std::string& typeName,
    parser::pObject_t obj, entityId_t parentId, const Matrix& parentTransform)
    : CSpawn(entityId, CSpawnKind::SPAWNABLE),
      spawnPoint(spawnPoint),
      typeName(typeName),
      object(std::move(obj)),
      parentId(parentId),
      parentTransform(parentTransform) {}

  entityId_t spawnPoint;
  std::string typeName;
  parser::pObject_t object;
  entityId_t parentId;
  Matrix parentTransform;
  double delay = 5;

  virtual ~CSpawnable() override {}
};

struct CSpawnPoint : public CSpawn {
  CSpawnPoint(entityId_t entityId)
    : CSpawn(entityId, CSpawnKind::SPAWN_POINT) {}

  virtual ~CSpawnPoint() override {}
};

typedef std::unique_ptr<CSpawn> pCSpawn_t;
typedef std::unique_ptr<CSpawnPoint> pCSpawnPoint_t;
typedef std::unique_ptr<CSpawnable> pCSpawnable_t;

class EntityManager;
class RootFactory;

class SpawnSystem : public System {
  public:
    SpawnSystem(EntityManager& entityManager, RootFactory& rootFactory)
      : m_entityManager(entityManager),
        m_rootFactory(rootFactory) {}

    virtual void update() override {}
    virtual void handleEvent(const GameEvent& event) override;
    virtual void handleEvent(const GameEvent& event,
      const std::set<entityId_t>& entities) override {}

    virtual void addComponent(pComponent_t component) override;
    virtual bool hasComponent(entityId_t entityId) const override;
    virtual Component& getComponent(entityId_t entityId) const override;
    virtual void removeEntity(entityId_t id) override;

    virtual ~SpawnSystem() override {}

  private:
    EntityManager& m_entityManager;
    RootFactory& m_rootFactory;

    std::map<entityId_t, pCSpawnPoint_t> m_spawnPoints;
    std::map<entityId_t, pCSpawnable_t> m_spawnables;
};


#endif
