#ifndef __PROCALC_FRAGMENTS_F_MAZE_3D_SPEC_HPP__
#define __PROCALC_FRAGMENTS_F_MAZE_3D_SPEC_HPP__


#include "fragment_spec.hpp"


struct FMaze3dSpec : public FragmentSpec {
  FMaze3dSpec()
    : FragmentSpec("FMaze3d", {}) {}

  std::string mapFile;
  int width = 320;
  int height = 240;
  int frameRate = 60;
};


#endif
