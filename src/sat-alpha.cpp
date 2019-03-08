#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../lib/home_away_pattern_sets.h"

int main(int argc, char **argv) {
  int nteams = atoi(argv[1]);
  int nslots = nteams - 1;
  HomeAwayPatternSets hap(nteams); // HAP
  hap.print_mode = kONELINE;

  // Read HAP from standard input
  for (int t = 0; t < nteams; ++t)
    for (int s = 0; s < nslots; ++s) {
      int v;
      scanf("%d", &v);
      hap.set(t, s, (HomeAway)v);
    }

  // print HAP if it satisfy Miyashiro condition
  if (hap.satisfyAlphaWithAnySubset())
    std::cout << hap << std::endl;
  else
    return 1;

  return 0;
}
