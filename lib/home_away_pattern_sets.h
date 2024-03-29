#ifndef HOME_AWAY_PATTERN_SETS_H_
#define HOME_AWAY_PATTERN_SETS_H_

#include <set>
#include <string>
#include <vector>

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
  std::vector<int> perm_rows;
  std::vector<int> perm_cols;
  std::vector<std::vector<HomeAway>> table;
public:
  const int nteams;
  HomeAwayPatternSetsPrintMode print_mode = (HomeAwayPatternSetsPrintMode)0;
  HomeAwayPatternSets(int n);
  HomeAwayPatternSets(const HomeAwayPatternSets &other);
  void set(const HomeAwayPatternSets &other);
  void set(int t, int s, HomeAway val);
  HomeAway get(int t, int s) const;
  void print();
  void print(HomeAwayPatternSetsPrintMode mode);
  void transformRepresentative();
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
  friend std::ostream& operator<<(std::ostream& os, const HomeAwayPatternSets &hap);
};


// ===============================================
// for unordered_map
// ===============================================
namespace std
{
  template<>
  class hash<HomeAwayPatternSets>
  {
  public:
    size_t operator()(const HomeAwayPatternSets &obj) const
    {
        std::string result = "";
        int cnt = 0, num = 0;
        for (int t = 0; t < obj.nteams; ++t) {
          for (int s = 0; s < obj.nteams-1; ++s) {
            if ((unsigned)cnt >= 8*sizeof(char)) {
              result += (char)num;
              num = cnt = 0;
            }
            num = (num << 1) + obj.get(t, s);
            ++cnt;
          }
        }
        return std::hash<std::string>{}(result + (char)num);
    }
  };
}


#endif // HOME_AWAY_PATTERN_SETS_H_
