#include <random>
#include <cmath>
#include <limits>
#include <vector>
#include <sstream>
#include <QRect>
#include <QRectF>
#include <QSize>
#include "utils.hpp"


using std::string;
using std::stringstream;
using std::istream;
using std::ostream;
using std::vector;


#ifdef DEBUG
ostream& operator<<(ostream& os, const QRect& rect) {
  os << rect.x() << ", " << rect.y() << ", " << rect.width() << ", " << rect.height();
  return os;
}
ostream& operator<<(ostream& os, const QRectF& rect) {
  os << rect.x() << ", " << rect.y() << ", " << rect.width() << ", " << rect.height();
  return os;
}
ostream& operator<<(ostream& os, const QPoint& p) {
  os << p.x() << ", " << p.y();
  return os;
}
ostream& operator<<(ostream& os, const QPointF& p) {
  os << p.x() << ", " << p.y();
  return os;
}
std::ostream& operator<<(std::ostream& os, const QSize& sz) {
  os << "(" << sz.width() << ", " << sz.height() << ")";
  return os;
}
#endif

//===========================================
// randomSeed
//===========================================
#ifdef __APPLE__
#include <chrono>

using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

long randomSeed() {
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
#else
long randomSeed() {
  static std::random_device rd;
  return rd();
}
#endif

//===========================================
// readString
//===========================================
string readString(istream& is) {
  int nBytes = 0;
  is.read(reinterpret_cast<char*>(&nBytes), sizeof(nBytes));

  vector<char> buf(nBytes);
  is.read(buf.data(), nBytes);

  return string(buf.data(), nBytes);
}

//===========================================
// writeString
//===========================================
void writeString(ostream& os, const string& s) {
  size_t nBytes = s.size();

  os.write(reinterpret_cast<const char*>(&nBytes), sizeof(nBytes));
  os.write(s.data(), nBytes);
}

//===========================================
// splitString
//===========================================
vector<string> splitString(const string& s, char delim) {
  vector<string> v;
  stringstream ss(s);

  string tok;
  while (std::getline(ss, tok, delim)) {
    v.push_back(tok);
  }

  return v;
}
