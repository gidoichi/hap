#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include "../lib/home_away_pattern_sets.h"

void recur(HomeAwayPatternSets &hap, int ipattern, int target);
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
  for (int ipattern = 0; ipattern <= ((1 << (n-1))-1); ipattern = (ipattern << 1) + 1) {
    // チーム0のhome away patternをipatternsに設定
    for (int s = 0; s < n-1; ++s)
      hap.set(0, s, (HomeAway)((ipattern >> s) % 2));

    recur(hap, ipattern+1, 1);
  }

  return 0;
}


/**
 * 再帰を用いて，alphaを満たしかつ行の入れ替えを行ったHAPを出力
 * @param hap targetより前のチームが割り当て済みのHAP
 * @param ipattern home away patternの10進展開表現
 * @param target チームtarget以降が全て変更される
 */
void recur(HomeAwayPatternSets &hap, int ipattern, int target) {
  // チームtarget以上の全てのチームに対し，あらゆるhome away patternで検証
  for (; ipattern < (1 << (n - 1)); ++ipattern){

    // チームtargetのhome away patternをipatternsに設定
    for (int s = 0; s < n-1; ++s)
      hap.set(target, s, (HomeAway)((ipattern >> s) % 2));

    // alphaを満たしたら続行
    if (target < 2 ||
        satAlphaIncludeLastTeam(hap, std::set<int>(teamset.begin(),
                                                   std::next(teamset.begin(), target + 1)))) {
      if (target >= n-1) {
        // 全てのhome away patternが割り当てられたらalphaを満たすものを表示
        std::cout << hap << std::endl;
      } else {
        // 未割り当があれば再帰
        recur(hap, ipattern + 1, target + 1);
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
