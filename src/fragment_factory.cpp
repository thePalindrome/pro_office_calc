#include "fragment_factory.hpp"
#include "exception.hpp"
#include "utils.hpp"
#include "fragments/f_main/f_main.hpp"
#include "fragments/f_main/f_countdown_to_start/f_countdown_to_start.hpp"
#include "fragments/f_main/f_menu_bar/f_menu_bar.hpp"
#include "fragments/f_main/f_calculator/f_calculator.hpp"
#include "fragments/f_main/f_calculator/f_normal_calc_trigger/f_normal_calc_trigger.hpp"
#include "fragments/f_main/f_shuffled_calc/f_shuffled_calc.hpp"


using std::string;


//===========================================
// constructFragment
//===========================================
Fragment* constructFragment(const string& name, Fragment& parent, FragmentData& parentData) {
  DBG_PRINT("constructFragment(), name=" << name << "\n");

  if (name == "FMenuBar") {
    return new FMenuBar(parent, parentData);
  }
  else if (name == "FCalculator") {
    return new FCalculator(parent, parentData);
  }
  else if (name == "FNormalCalcTrigger") {
    return new FNormalCalcTrigger(parent, parentData);
  }
  else if (name == "FShuffledCalc") {
    return new FShuffledCalc(parent, parentData);
  }
  else if (name == "FCountdownToStart") {
    return new FCountdownToStart(parent, parentData);
  }

  EXCEPTION("Cannot construct fragment with unrecognised name '" << name << "'\n");
}
