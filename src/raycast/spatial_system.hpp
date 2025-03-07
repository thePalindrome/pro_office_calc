#ifndef __PROCALC_RAYCAST_SPATIAL_SYSTEM_HPP__
#define __PROCALC_RAYCAST_SPATIAL_SYSTEM_HPP__


#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <set>
#include <memory>
#include <QImage>
#include "raycast/scene_graph.hpp"
#include "raycast/system.hpp"


namespace tinyxml2 { class XMLElement; }
namespace parser { struct Object; }


struct Intersection {
  Intersection(CSpatialKind kind, CSpatialKind parentKind)
    : kind(kind),
      parentKind(parentKind) {}

  CSpatialKind kind;
  CSpatialKind parentKind;
  entityId_t entityId;
  Point point_rel;
  Point point_wld;
  Point viewPoint;
  double distanceFromOrigin;
  double distanceAlongTarget;
  entityId_t zoneB;
  entityId_t zoneA;

  double height = 0;
  std::pair<Range, Range> heightRanges;
};

typedef std::unique_ptr<Intersection> pIntersection_t;

struct EChangedZone : public GameEvent {
  EChangedZone(entityId_t entityId, entityId_t oldZone, entityId_t newZone,
    const std::set<entityId_t>& zonesLeft, const std::set<entityId_t>& zonesEntered)
    : GameEvent("entity_changed_zone"),
      entityId(entityId),
      oldZone(oldZone),
      newZone(newZone),
      zonesLeft(zonesLeft),
      zonesEntered(zonesEntered) {}

  entityId_t entityId;
  entityId_t oldZone;
  entityId_t newZone;
  std::set<entityId_t> zonesLeft;
  std::set<entityId_t> zonesEntered;
};

struct EPlayerMove : public GameEvent {
  EPlayerMove(const Player& player)
    : GameEvent("player_move"),
      player(player) {}

  const Player& player;
};

struct EPlayerActivateEntity : public GameEvent {
  EPlayerActivateEntity(const Player& player)
    : GameEvent("player_activate_entity"),
      player(player) {}

  const Player& player;
  std::set<entityId_t> inRadius;
  std::set<entityId_t> lookingAt;
};

class EntityManager;
class TimeService;

class SpatialSystem : public System {
  public:
    SpatialSystem(EntityManager& entityManager, TimeService& timeService, double frameRate);

    SceneGraph sg;

    void connectZones();

    void update() override;
    void handleEvent(const GameEvent& event) override;
    void handleEvent(const GameEvent&, const std::set<entityId_t>&) override {}

    void addComponent(pComponent_t component) override;
    bool hasComponent(entityId_t entityId) const override;
    void removeEntity(entityId_t id) override;

    void moveEntity(entityId_t id, Vec2f dv, double heightAboveFloor = 0);
    void relocateEntity(entityId_t id, CZone& zone, const Point& point);

    std::set<entityId_t> entitiesInRadius(const CZone& zone, const Point& pos, double radius,
      double heightAboveFloor = 0.0) const;

    std::vector<pIntersection_t> entitiesAlongRay(const CZone& zone, const Point& pos,
      const Vec2f& dir, const Matrix& matrix, double distance = 10000) const;
    std::vector<pIntersection_t> entitiesAlongRay(const Vec2f& dir, double distance = 10000) const;

    std::vector<pIntersection_t> entitiesAlong3dRay(const CZone& zone, const Point& pos,
      double height, const Vec2f& dir, double vAngle, const Matrix& matrix,
      double distance = 10000) const;
    std::vector<pIntersection_t> entitiesAlong3dRay(const Vec2f& dir, double camSpaceVAngle,
      double distance = 10000) const;

    std::set<entityId_t> getAncestors(entityId_t entityId) const;

    std::vector<Point> shortestPath(entityId_t entityA, entityId_t entityB, double radius) const;
    std::vector<Point> shortestPath(const Point& A, const Point& B, double radius) const;

    void vRotateCamera(double da);
    void hRotateCamera(double da);
    void movePlayer(const Vec2f& v);

    CZone& zone(entityId_t entity);
    const CZone& constZone(entityId_t entity) const;

    inline CSpatial& getComponent(entityId_t entityId) const override {
      return *GET_VALUE(m_components, entityId);
    }

  private:
    bool isRoot(const CSpatial& c) const;
    void removeEntity_r(entityId_t id);
    void crossZones(entityId_t entityId, entityId_t oldZone, entityId_t newZone);
    void connectSubzones(CZone& zone);
    bool areTwins(const CSoftEdge& se1, const CSoftEdge& se2) const;
    bool isAncestor(entityId_t a, entityId_t b) const;
    void findIntersections_r(const Point& point, const Vec2f& dir, const Matrix& matrix,
      const CZone& zone, const CSpatial& parent, std::vector<pIntersection_t>& intersections,
      std::vector<entityId_t>& visited, double cullNearerThan, double& cullFartherThan) const;
    void addChildToComponent(CSpatial& parent, pCSpatial_t child);
    bool removeChildFromComponent(CSpatial& parent, const CSpatial& child, bool keepAlive = false);

    inline CZone& getCurrentZone() const {
      return DYNAMIC_CAST<CZone&>(getComponent(sg.player->region()));
    }

    void buoyancy();
    void gravity();

    EntityManager& m_entityManager;
    TimeService& m_timeService;

    double m_frameRate;

    std::map<entityId_t, CSpatial*> m_components;
    std::map<entityId_t, std::set<CSpatial*>> m_entityChildren;

    Vec2i m_playerCell;
};

std::ostream& operator<<(std::ostream& os, CSpatialKind kind);


#endif
