#include <gtest/gtest.h>
#include <list>
#include <set>
#include <fragments/f_main/f_menu_bar/f_settings_dialog/f_raycast/scene.hpp>
#include <fragments/f_main/f_menu_bar/f_settings_dialog/f_raycast/renderer.hpp>
#include <utils.hpp>


using std::list;
using std::set;
using std::unique_ptr;


class RendererTest : public testing::Test {
  public:
    virtual void SetUp() override {}

    virtual void TearDown() override {}
};

TEST_F(RendererTest, findIntersections_r_singleRegion) {
  list<unique_ptr<Edge>> edges;

  Size viewport;
  viewport.x = 10.0 * 320.0 / 240.0;
  viewport.y = 10.0;

  Camera camera(viewport.x, DEG_TO_RAD(60), DEG_TO_RAD(50));
  camera.pos = Point(100, 100);
  camera.angle = DEG_TO_RAD(89.9);
  camera.height = 50;

  Region* region = new Region(Component::getNextId(), -1);

  Wall* wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(1, 1), Point(200, 1));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(200, 1), Point(200, 200));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(200, 200), Point(0, 200));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(0, 200), Point(1, 1));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  //buildPolygon(*region);

  int screenW_px = 640;
  double hWorldUnitsInPx = screenW_px / viewport.x;

  int screenX_px = 321;
  double projX_wd = static_cast<double>(screenX_px - screenW_px / 2) / hWorldUnitsInPx;
  Vec2f r(camera.F, projX_wd);
  LineSegment ray(Point(0, 0), Point(r.x * 999.9, r.y * 999.9));

  CastResult result;
  set<const Region*> visitedRegions;
  set<const JoiningEdge*> visitedJoiningEdges;
  findIntersections_r(camera, ray, *region, result, visitedRegions, visitedJoiningEdges);

  result.intersections.sort([](const pIntersection_t& a, const pIntersection_t& b) {
    return a->distanceFromCamera < b->distanceFromCamera;
  });

  ASSERT_EQ(1, result.intersections.size());
}

TEST_F(RendererTest, findIntersections_r_nestedRegions) {
  list<unique_ptr<Edge>> edges;

  Size viewport;
  viewport.x = 10.0 * 320.0 / 240.0;
  viewport.y = 10.0;

  Camera camera(viewport.x, DEG_TO_RAD(60), DEG_TO_RAD(50));
  camera.pos = Point(10, 10);
  camera.angle = DEG_TO_RAD(45);
  camera.height = 50;

  Region* region = new Region(Component::getNextId(), -1);

  Wall* wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(1, 1), Point(200, 1));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(200, 1), Point(200, 200));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(200, 200), Point(0, 200));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(0, 200), Point(1, 1));
  wall->texture = "light_bricks";
  region->edges.push_back(wall);

  Region* subregion = new Region(Component::getNextId(), region->entityId());

  wall = new Wall(Component::getNextId(), subregion->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(51, 51), Point(150, 1));
  wall->texture = "light_bricks";
  subregion->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), subregion->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(150, 51), Point(150, 150));
  wall->texture = "light_bricks";
  subregion->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), subregion->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(150, 150), Point(50, 150));
  wall->texture = "light_bricks";
  subregion->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), subregion->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(50, 150), Point(51, 51));
  wall->texture = "light_bricks";
  subregion->edges.push_back(wall);

  region->children.push_back(unique_ptr<Region>(subregion));

  //buildPolygon(*region);

  int screenW_px = 640;
  double hWorldUnitsInPx = screenW_px / viewport.x;

  int screenX_px = 321;
  double projX_wd = static_cast<double>(screenX_px - screenW_px / 2) / hWorldUnitsInPx;
  Vec2f r(camera.F, projX_wd);
  LineSegment ray(Point(0, 0), Point(r.x * 999.9, r.y * 999.9));

  CastResult result;
  set<const Region*> visitedRegions;
  set<const JoiningEdge*> visitedJoiningEdges;
  findIntersections_r(camera, ray, *region, result, visitedRegions, visitedJoiningEdges);

  result.intersections.sort([](const pIntersection_t& a, const pIntersection_t& b) {
    return a->distanceFromCamera < b->distanceFromCamera;
  });

  ASSERT_EQ(3, result.intersections.size());
}

TEST_F(RendererTest, findIntersections_r_joinedRegions) {
  list<unique_ptr<Edge>> edges;

  Size viewport;
  viewport.x = 10.0 * 320.0 / 240.0;
  viewport.y = 10.0;

  Camera camera(viewport.x, DEG_TO_RAD(60), DEG_TO_RAD(50));
  camera.pos = Point(100, 100);
  camera.angle = DEG_TO_RAD(0.1);
  camera.height = 50;

  Region* rootRegion = new Region(Component::getNextId(), -1);
  Region* region1 = new Region(Component::getNextId(), rootRegion->entityId());
  Region* region2 = new Region(Component::getNextId(), rootRegion->entityId());

  rootRegion->children.push_back(unique_ptr<Region>(region1));
  rootRegion->children.push_back(unique_ptr<Region>(region2));

  Wall* wall = new Wall(Component::getNextId(), region1->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(0, 0), Point(200, 1));
  wall->texture = "light_bricks";
  region1->edges.push_back(wall);

  JoiningEdge* je = new JoiningEdge(Component::getNextId(), region1->entityId());
  edges.push_back(std::unique_ptr<JoiningEdge>(je));
  je->lseg = LineSegment(Point(200, 1), Point(201, 201));
  je->topTexture = "light_bricks";
  je->bottomTexture = "dark_bricks";
  region1->edges.push_back(je);

  wall = new Wall(Component::getNextId(), region1->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(201, 201), Point(1, 200));
  wall->texture = "light_bricks";
  region1->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region1->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(1, 200), Point(0, 0));
  wall->texture = "light_bricks";
  region1->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region2->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(200, 1), Point(400, 2));
  wall->texture = "light_bricks";
  region2->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region2->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(400, 2), Point(401, 202));
  wall->texture = "light_bricks";
  region2->edges.push_back(wall);

  wall = new Wall(Component::getNextId(), region2->entityId());
  edges.push_back(std::unique_ptr<Wall>(wall));
  wall->lseg = LineSegment(Point(401, 202), Point(201, 201));
  wall->texture = "light_bricks";
  region2->edges.push_back(wall);

  region2->edges.push_back(je);
  je->regionA = region1;
  je->regionB = region2;

  //buildPolygon(*region);

  int screenW_px = 640;
  double hWorldUnitsInPx = screenW_px / viewport.x;

  int screenX_px = 321;
  double projX_wd = static_cast<double>(screenX_px - screenW_px / 2) / hWorldUnitsInPx;
  Vec2f r(camera.F, projX_wd);
  LineSegment ray(Point(0, 0), Point(r.x * 999.9, r.y * 999.9));

  CastResult result;
  set<const Region*> visitedRegions;
  set<const JoiningEdge*> visitedJoiningEdges;
  findIntersections_r(camera, ray, *region1, result, visitedRegions, visitedJoiningEdges);

  result.intersections.sort([](const pIntersection_t& a, const pIntersection_t& b) {
    return a->distanceFromCamera < b->distanceFromCamera;
  });

  ASSERT_EQ(2, result.intersections.size());
}
