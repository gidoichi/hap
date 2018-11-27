#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include "home_away_pattern_sets.h"

// 比較や代入時にチーム数の異なるHAPを考慮しない

HomeAwayPatternSets::HomeAwayPatternSets(int n)
  : nteams(n)
{
  perm_rows = new int[n];
  perm_cols = new int[n-1];
  table = new HomeAway*[n];
  for (int i = 0; i < n; ++i) {
    perm_rows[i] = i;
    table[i] = new HomeAway[n];
  }
  for (int i = 0; i < n-1; ++i) {
    perm_cols[i] = i;
  }
}

HomeAwayPatternSets::HomeAwayPatternSets(const HomeAwayPatternSets &other)
  : HomeAwayPatternSets(other.nteams)
{
  *this = other;
}

HomeAwayPatternSets::~HomeAwayPatternSets()
{
  delete[] perm_rows;
  delete[] perm_cols;
  for (int i = 0; i < nteams; ++i) {
    delete[] table[i];
  }
  delete[] table;
}

void HomeAwayPatternSets::set(const HomeAwayPatternSets &other)
{
  *this = other;
}

void HomeAwayPatternSets::set(int t, int s, HomeAway val)
{
  table[t][s] = val;
}

HomeAway HomeAwayPatternSets::get(int t, int s) const
{
  return table[perm_rows[t]][perm_cols[s]];
}

void HomeAwayPatternSets::print() const
{
  print((HomeAwayPatternSetsPrintMode)0);
}

void HomeAwayPatternSets::print(HomeAwayPatternSetsPrintMode mode) const
{
  switch(mode) {
  case kHAP01: // HAPを0,1表現で出力
    static char hapchar[] = {[kAway] = '0', [kHome] = '1'};
    for (int t = 0; t < nteams; ++t) {
      for (int s = 0; s < nteams - 1; ++s) {
        printf("%c ", hapchar[get(t, s)]);
      }
      puts("");
    }
    break;

  case kONELINE: // HAPを1行で出力
    for (int t = 0; t < nteams; ++t) {
      for (int s = 0; s < nteams - 1; ++s) {
        printf("%c ", hapchar[get(t, s)]);
      }
    }
    puts("");
    break;
  }
}

void HomeAwayPatternSets::swapTeams(int t1, int t2)
{
  int t = perm_rows[t1];
  perm_rows[t1] = perm_rows[t2];
  perm_cols[t2] = t;
}

void HomeAwayPatternSets::swapSlots(int s1, int s2)
{
  int t = perm_cols[s1];
  perm_cols[s1] = perm_cols[s2];
  perm_cols[s2] = t;
}

// alpha(teams) := sum_{s in slots} min(#ofA(teams, s), #ofH(teams, s))
//                 - |teams| (|teams| - 1) / 2
// @return alpha(teams) > 0
bool HomeAwayPatternSets::satisfyAlpha(const std::set<int> teams) const
{
  unsigned sum = 0;
  for (int s = 0; s < nteams-1; ++s) {
    int nhome = 0, naway = 0; // #ofH, #ofA
    for (int t : teams) {
      if (get(t, s) == kHome) ++nhome;
      else ++naway;
    }
    sum += std::min(nhome, naway);
  }
  return (sum >= teams.size() * (teams.size() - 1) / 2);
}

bool HomeAwayPatternSets::satisfyAlphaWithAnySubset(const std::set<int> teams) const
{
  return satisfyAlphaWithAnySubsetOf(teams);
}

bool HomeAwayPatternSets::satisfyAlphaWithAnySubset() const
{
  std::set<int> teams;
  for (int i = 0; i < nteams; ++i) teams.insert(i);
  return satisfyAlphaWithAnySubsetOf(teams);
}

// teamsの任意の部分集合について，satisfyAlphaがtrueになるかどうかの判定
bool HomeAwayPatternSets::satisfyAlphaWithAnySubsetOf(const std::set<int> teams) const
{
  std::set<int>::iterator itr = teams.begin();
  for (int subset = (1 << teams.size()) - 1; subset > 0; --subset) {
    std::set<int> ts; // 部分集合

    // 部分集合として採用するかどうかは変数subsetのビットが立ってるかで判断
    for (int i = 0; (unsigned)i < teams.size(); ++i) {
      if ((subset >> i) % 2 == 1) ts.insert(*std::next(itr, i));
    }

    if (!satisfyAlpha(ts)) return false;
  }
  return true;
}

HomeAwayPatternSets& HomeAwayPatternSets::operator=(const HomeAwayPatternSets &other)
{
  for (int t = 0; t < nteams; ++t) {
    for (int s = 0; s < nteams-1; ++s) {
      table[t][s] = other.get(t, s);
    }
  }
  return *this;
}

bool HomeAwayPatternSets::operator==(const HomeAwayPatternSets &other) const
{
  for (int t = 0; t < nteams; ++t) {
    for (int s = 0; s < nteams-1; ++s) {
      if (this->get(t, s) != other.get(t, s)) return false;
    }
  }
  return true;
}

bool HomeAwayPatternSets::operator!=(const HomeAwayPatternSets &other) const
{
  return !(*this == other);
}
