#ifndef HOME_AWAY_PATTERN_SETS_H_
#define HOME_AWAY_PATTERN_SETS_H_

#include <set>
#include <string>

// ===============================================
// enum
// ===============================================
enum HomeAway {kAway, kHome};
enum HomeAwayPatternSetsPrintMode {kHAP01, kONELINE};

// ===============================================
// class HomeAwayPatternSets
// ===============================================
class HomeAwayPatternSets {
private:
  int *perm_rows;
  int *perm_cols;
  HomeAway **table;
public:
  const int nteams;
  HomeAwayPatternSets(int n);
  HomeAwayPatternSets(const HomeAwayPatternSets &other);
  ~HomeAwayPatternSets();
  void set(const HomeAwayPatternSets &other);
  void set(int t, int s, HomeAway val);
  HomeAway get(int t, int s) const;
  void print() const;
  void print(HomeAwayPatternSetsPrintMode mode) const;
  void swapTeams(int t1, int t2);
  void swapSlots(int s1, int s2);
  bool satisfyAlpha(const std::set<int> teams) const;
  [[deprecated("please use satisfyAlphaWithAnySubsetOf function")]]
  bool satisfyAlphaWithAnySubset(const std::set<int> teams) const;
  bool satisfyAlphaWithAnySubset() const;
  bool satisfyAlphaWithAnySubsetOf(const std::set<int> teams) const;
  HomeAwayPatternSets& operator=(const HomeAwayPatternSets &other);
  bool operator==(const HomeAwayPatternSets &other) const;
  bool operator!=(const HomeAwayPatternSets &other) const;
};

#endif // HOME_AWAY_PATTERN_SETS_H_
