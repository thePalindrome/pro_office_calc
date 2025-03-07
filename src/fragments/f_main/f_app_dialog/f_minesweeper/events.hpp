#ifndef __PROCALC_FRAGMENTS_F_MINESWEEPER_EVENTS_HPP__
#define __PROCALC_FRAGMENTS_F_MINESWEEPER_EVENTS_HPP__


#include <set>
#include <ostream>
#include <vector>
#include "event.hpp"


namespace doomsweeper {


struct Coord {
  int row;
  int col;
};

inline std::ostream& operator<<(std::ostream& out, const Coord& coord) {
  out << "(" << coord.row << ", " << coord.col << ")";
  return out;
}

inline bool operator==(const Coord& lhs, const Coord& rhs) {
  return lhs.row == rhs.row && lhs.col == rhs.col;
}

inline bool operator!=(const Coord& lhs, const Coord& rhs) {
  return !(lhs == rhs);
}

inline bool operator<(const Coord& lhs, const Coord& rhs) {
  if (lhs.row == rhs.row) {
    return lhs.col < rhs.col;
  }
  else {
    return lhs.row < rhs.row;
  }
}

struct MinesweeperSetupEvent : public Event {
  MinesweeperSetupEvent(const std::set<Coord>& coords)
    : Event("doomsweeper/minesweeperSetupComplete"),
      mineCoords(coords) {}

  std::set<Coord> mineCoords;
};

struct CellEnteredEvent : public Event {
  CellEnteredEvent(const Coord& coords)
    : Event("doomsweeper/cellEntered"),
      coords(coords) {}

  Coord coords;
};

struct InnerCellEnteredEvent : public Event {
  InnerCellEnteredEvent(const Coord& coords)
    : Event("doomsweeper/innerCellEntered"),
      coords(coords) {}

  Coord coords;
};

struct CommandsGeneratedEvent : public Event {
  CommandsGeneratedEvent(const std::vector<std::vector<std::string>>& commands)
    : Event("doomsweeper/commandsGenerated"),
      commands(commands) {}

  std::vector<std::vector<std::string>> commands;
};


}


#endif
