//////////////////////////////////////////////////////////////////////
// チーム数8以上でメモリエラー
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <unordered_map>
#include "../lib/home_away_pattern_sets.h"

void recur(HomeAwayPatternSets &hap, int patterns[], int target);
bool satAlphaIncludeLastTeam(HomeAwayPatternSets &hap, const std::set<int> &teams);

int n; // チーム数
std::set<int> teamset; // チーム集合
std::unordered_map<HomeAwayPatternSets, int> hapmap;

int main(int argc, char **argv) {
  n = atoi(argv[1]);
  int hapatterns[n]; // hapatterns[i]: チームiのhome away patternの数値表現
  HomeAwayPatternSets hap(n); // HAP

  // init
  for (int i = 0; i < n; ++i) {
    hapatterns[i] = 0;
    teamset.insert(i);
  }

  // メイン処理
  hap.print_mode = kHAP01;
  recur(hap, hapatterns, 0);

  return 0;
}


/**
 * 再帰を用いて，alphaを満たしかつ行の入れ替えを行ったHAPを出力
 * @param hap チームiのhome away patternがpatterns[i]に則っているHAPインスタンス
 * @param patterns home away patternの10進展開表現
 * @param target チームtarget以降が全て変更される
 */
void recur(HomeAwayPatternSets &hap, int patterns[], int target) {
  // チームtarget以上の全てのチームに対し，あらゆるhome away patternで検証
  for (patterns[target] = (target <= 0) ? 0 : (patterns[target-1] + 1);
       patterns[target] < (1 << (n - 1));
       ++patterns[target]){

    // チームtargetのhome away patternをpatterns[target]に設定
    for (int s = 0; s < n-1; ++s)
      hap.set(target, s, (HomeAway)((patterns[target] >> s) % 2));

    if (target >= n-1) {
      // 全てのhome away patternが割り当てられたらalphaを満たすものを表示
      if (hap.satisfyAlphaWithAnySubsetOf(std::set<int>(teamset.begin(),
                                                        teamset.end()))) {
        HomeAwayPatternSets haptrans(hap);
        haptrans.transformRepresentative();
        if (++hapmap[haptrans] == 1) {
          std::cout << haptrans << std::endl;
        }
      }

    } else if (target < 2) {
      // alphaを必ず満たす場合は無条件に再帰
      recur(hap, patterns, target + 1);

    } else {
      // 割り当てたチームについてalphaを満たすか検証し，満たすものについて再帰
      if (satAlphaIncludeLastTeam(hap, std::set<int>(teamset.begin(),
                                                     std::next(teamset.begin(), target + 1)))) {
        recur(hap, patterns, target + 1);
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
