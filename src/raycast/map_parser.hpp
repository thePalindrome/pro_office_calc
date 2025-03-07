#ifndef __PROCALC_RAYCAST_MAP_PARSER_HPP__
#define __PROCALC_RAYCAST_MAP_PARSER_HPP__


#include <list>
#include <vector>
#include <map>
#include <memory>
#include <tinyxml2.h>
#include "raycast/geometry.hpp"
#include "raycast/component.hpp"


namespace parser {


struct Path {
  std::vector<Point> points;
  bool closed = false;
};

struct Object;

typedef std::unique_ptr<Object> pObject_t;

struct Object {
  Object(const Object* parent)
    : parent(parent) {}

  // For printing debug information on parse error
  const Object* parent = nullptr;

  Matrix groupTransform;
  Matrix pathTransform;
  Path path;
  std::string type;
  std::map<std::string, std::string> dict;
  std::list<pObject_t> children;

  Object* clone(const Object* parent = nullptr) const {
    Object* cpy = new Object(nullptr);
    cpy->parent = parent;
    cpy->groupTransform = groupTransform;
    cpy->pathTransform = pathTransform;
    cpy->path = path;
    cpy->type = type;
    cpy->dict = dict;

    for (auto& c: children) {
      cpy->children.push_back(pObject_t(c->clone(cpy)));
    }

    return cpy;
  }
};

void parse(const std::string& file, std::list<pObject_t>& objects);
Object* firstObjectOfType(const std::list<pObject_t>& objects, const std::string& type);

struct PathStreamToken {
  enum { SET_RELATIVE, SET_ABSOLUTE, CLOSE_PATH, POINT } kind;
  Point p;
};

struct ParseError {
  const Object* object;
  std::string message;
};

typedef std::list<ParseError> ParseErrors;

Matrix transformFromTriangle(const Path& path);
PathStreamToken getNextToken(std::istream& is);
void constructPath(const tinyxml2::XMLElement& e, Path& path);
std::pair<std::string, std::string> parseKvpString(const std::string& s);
void extractKvPairs(const tinyxml2::XMLElement& e, std::map<std::string, std::string>& kv);
Matrix parseTranslateTransform(const std::string& s);
Matrix parseMatrixTransform(const std::string& s);
Matrix parseTransform(const std::string& s);
void extractGeometry(const tinyxml2::XMLElement& node, Path& path, Matrix& transform);
Object* constructObject_r(const Object* parent, const tinyxml2::XMLElement& node,
  ParseErrors& errors);


}


const std::string& getValue(const std::map<std::string, std::string>& m, const std::string& key);
const std::string& getValue(const std::map<std::string, std::string>& m, const std::string& key,
  const std::string& default_);
entityId_t makeIdForObj(const parser::Object& obj);


#endif
