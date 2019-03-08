#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "home_away_pattern_sets.h"

// 比較や代入時にチーム数の異なるHAPを考慮しない

HomeAwayPatternSets::HomeAwayPatternSets(int n)
  : nteams(n)
{
  perm_rows.resize(n);
  perm_cols.resize(n-1);
  table.resize(n);
  for (int i = 0; i < n; ++i) {
    perm_rows[i] = i;
    table[i].resize(n);
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

void HomeAwayPatternSets::print()
{
  print((HomeAwayPatternSetsPrintMode)0);
}

void HomeAwayPatternSets::print(HomeAwayPatternSetsPrintMode mode)
{
  HomeAwayPatternSetsPrintMode current = print_mode;
  print_mode = mode;
  std::cout << *this << std::endl;
  print_mode = current;
}

void HomeAwayPatternSets::transformRepresentative()
{
  constexpr bool DEBUG = false; // print for DEBUG
  std::vector<std::vector<int>>
    *row_feature, *pre_row_feature,
    *col_feature, *pre_col_feature;

  // 初期化
  pre_row_feature = new std::vector<std::vector<int>>;
  pre_col_feature = new std::vector<std::vector<int>>;
  row_feature = new std::vector<std::vector<int>>(nteams, std::vector<int>(1, 0));
  col_feature = new std::vector<std::vector<int>>(nteams-1, std::vector<int>(1, 0));

  int cnt = 0;
  if (DEBUG) print();
  while (true) {

    // 行の特徴量計算
    delete pre_row_feature;
    pre_row_feature = row_feature;
    row_feature = new std::vector<std::vector<int>>(nteams);
    for (int t = 0; t < nteams; ++t) {
      (*row_feature)[perm_rows[t]].reserve(nteams-1);
      for (int s = 0; s < nteams-1; ++s) {
        if (s == 0 || (s > 0 && (*col_feature)[perm_cols[s-1]] != (*col_feature)[perm_cols[s]]))
          (*row_feature)[perm_rows[t]].push_back(get(t, s));
        else
          *((*row_feature)[perm_rows[t]].end()-1) += get(t, s);
      }
    }

    if (DEBUG) {
      printf("row feat: ");
      for (int i = 0; i < nteams; ++i) {
        printf("{");
        for (int j : (*row_feature)[perm_rows[i]]) printf("%d,", j);
        printf("}, ");
      }
      puts("");
    }

    // 終了条件
    if (*pre_row_feature == *row_feature) return;

    // 行の並び替え
    for (unsigned fromi = 0, toi = fromi + 1; fromi < (*row_feature).size();
         fromi = toi) {
      while (toi < (*row_feature).size()
             && (*pre_row_feature)[fromi] == (*pre_row_feature)[toi]) ++toi;
      std::sort(perm_rows.begin() + fromi, perm_rows.begin() + toi,
                [&](int lhs, int rhs) {
                  return (*row_feature)[perm_rows[lhs]] < (*row_feature)[perm_rows[rhs]];
                });
    }

    if (DEBUG) {
      printf("row feat: ");
      for (int i = 0; i < nteams; ++i) {
        printf("{");
        for (int j : (*row_feature)[perm_rows[i]]) printf("%d,", j);
        printf("}, ");
      }
      puts("");
      print();
    }

    // 列の特徴量計算
    delete pre_col_feature;
    pre_col_feature = col_feature;
    col_feature = new std::vector<std::vector<int>>(nteams-1);
    for (int s = 0; s < nteams-1; ++s) {
      (*col_feature)[perm_cols[s]].reserve(nteams);
      for (int t = 0; t < nteams; ++t) {
        if (t == 0 || (t > 0 && (*row_feature)[perm_rows[t-1]] != (*row_feature)[perm_rows[t]]))
          (*col_feature)[perm_cols[s]].push_back(get(t, s));
        else
          *((*col_feature)[perm_cols[s]].end()-1) += get(t, s);
      }
    }

    if (DEBUG) {
      printf("col feat: ");
      for (int i = 0; i < nteams-1; ++i) {
        printf("{");
        for (int j : (*col_feature)[perm_cols[i]]) printf("%d,", j);
        printf("}, ");
      }
      puts("");
    }

    // 終了条件
    if (*pre_col_feature == *col_feature) return;

    // 列の並び替え
    for (unsigned fromi = 0, toi = fromi + 1; fromi < (*col_feature).size();
         fromi = toi) {
      while (toi < (*col_feature).size()
             && (*pre_col_feature)[fromi] == (*pre_col_feature)[toi]) ++toi;
      std::sort(perm_cols.begin() + fromi, perm_cols.begin() + toi,
                [&](int lhs, int rhs) {
                  return (*col_feature)[perm_cols[lhs]] < (*col_feature)[perm_cols[rhs]];
                });
    }

    if (DEBUG) {
      printf("col feat: ");
      for (int i = 0; i < nteams-1; ++i) {
        printf("{");
        for (int j : (*col_feature)[perm_cols[i]]) printf("%d,", j);
        printf("}, ");
      }
      puts("");
      print();
      if (++cnt >= 5) getchar();
    }
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
  print_mode = other.print_mode;
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

std::ostream& operator<<(std::ostream& os, const HomeAwayPatternSets &hap)
{
  switch(hap.print_mode) {
  case kHAP01: // HAPを0,1表現で出力
    static char hapchar[] = {[kAway] = '0', [kHome] = '1'};
    for (int t = 0; t < hap.nteams; ++t) {
      for (int s = 0; s < hap.nteams - 1; ++s) {
        os << hapchar[hap.get(t, s)];
        if (s < hap.nteams - 2) os << ' ';
      }
      os << std::endl;
    }
    break;

  case kONELINE: // HAPを1行で出力
    for (int t = 0; t < hap.nteams; ++t) {
      for (int s = 0; s < hap.nteams - 1; ++s) {
        os << hapchar[hap.get(t, s)] << ' ';
      }
    }
    break;
  }
  return os;
}
