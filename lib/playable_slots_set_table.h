#ifndef PLAYABLE_SLOTS_SET_TABLE_H_
#define PLAYABLE_SLOTS_SET_TABLE_H_

#include <set>
#include "home_away_pattern_sets.h"

class PlayableSlotsSetTable {
private:
  std::set<int> **table;
public:
  const int nteams;
  const HomeAwayPatternSets hap;
  PlayableSlotsSetTable(HomeAwayPatternSets &hap);
  ~PlayableSlotsSetTable();
  std::set<int> get(int t1, int t2) const;
  void print() const;
  void print(int mode) const;
};

#endif // HOME_AWAY_PATTERN_SETS_H_
