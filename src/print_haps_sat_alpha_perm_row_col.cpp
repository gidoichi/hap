#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include "../lib/home_away_pattern_sets.h"

#define HA    // H／Aの総入れ替えを行う
#define ALPHA // 条件alphaを満たすかチェックする

void set(HomeAwayPatternSets &hap, int team, int from, int to, int pattern);
int toint(HomeAwayPatternSets &hap, int team);
void recur(HomeAwayPatternSets &hap, int team, int i, const std::set<int> separate);
bool satAlphaIncludeLastTeam(HomeAwayPatternSets &hap, const std::set<int> &teams);

int n; // チーム数
std::set<int> teamset; // チーム集合

int main(int argc, char **argv) {
  n = atoi(argv[1]);
  HomeAwayPatternSets hap(n); // HAP

  // init
  for (int i = 0; i < n; ++i) {
    teamset.insert(i);
  }

  // メイン処理
  hap.print_mode = kONELINE;
  // hap.print_mode = kHAP01;
#ifdef HA
  set(hap, 0, 0, n, 0);
  recur(hap, 1, 0, std::set<int>{0,n-1});
#else
  recur(hap, 0, 0, std::set<int>{0,n-1});
#endif

  return 0;
}


// hapのteamのラウンド[from, to)にint^-1(pattern)を割当て
void set(HomeAwayPatternSets &hap, int team, int from, int to, int pattern)
{
  for (int i = 0; i < to - from; ++i) {
    hap.set(team, from+i, (HomeAway)((pattern >> i) % 2));
  }
}


int toint(HomeAwayPatternSets &hap, int team)
{
  int i = 0;
  for (int s = n-2; s >= 0; --s)
    i = (i<<1) + (int)hap.get(team, s);
  return i;
}


void recur(HomeAwayPatternSets &hap, int team, int i, const std::set<int> separate)
{
  int start = *std::next(separate.begin(), i);
  int end = *std::next(separate.begin(), i+1);
  for (int nh = 0; nh <= end - start; ++nh) {
    set(hap, team, start, end, (1 << nh) - 1);
    if (end >= n-1) {
      if (team <= 0 ||
#ifdef ALPHA
          (toint(hap, team-1) < toint(hap, team) &&
           satAlphaIncludeLastTeam(hap, std::set<int>(teamset.begin(),
           std::next(teamset.begin(), team + 1))))) {
#else
          toint(hap, team-1) <= toint(hap, team)) {
#endif
        if (team >= n-1) {
          std::cout << hap << std::endl;
        } else {
          std::set<int> sep(separate);
          sep.insert(start + nh);
          recur(hap, team+1, 0, sep);
        }
      }
    } else {
      std::set<int> sep(separate);
      if (sep.insert(start + nh).second) {
        recur(hap, team, i+2, sep);
      } else {
        recur(hap, team, i+1, sep);
      }
    }
  }
}


/**
 * チーム集合teamsに含まれるチームのうち，最も大きいチームを含むような
 * チームの部分集合において，alphaを満たすかどうかチェック
 */
bool satAlphaIncludeLastTeam(HomeAwayPatternSets &hap, const std::set<int> &teams) {
  std::set<int>::iterator itr = teams.begin();
  for (int subset = (1 << teams.size()) - 1; subset >= (1 << (teams.size() - 1)); --subset) {
    std::set<int> ts; // 部分集合

    // 部分集合として採用するかどうかは変数subsetのビットが立ってるかで判断
    for (int i = 0; (unsigned)i < teams.size(); ++i) {
      if ((subset >> i) % 2 == 1) ts.insert(*std::next(itr, i));
    }

    if (!hap.satisfyAlpha(ts)) return false;
  }
  return true;
}
