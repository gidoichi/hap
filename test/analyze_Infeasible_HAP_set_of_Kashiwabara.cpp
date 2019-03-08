#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include "../lib/home_away_pattern_sets.h"
#include "../lib/playable_slots_set_table.h"


void Print(const HomeAwayPatternSets &hap);
void Print(const PlayableSlotsSetTable &pst);

int main(void) {
  int nteams;
  std::ifstream file("data/Infeasible_HAP_set_of_Kashiwabara.txt", std::ifstream::in);
  if (!file.is_open()) {
    puts("File open error.");
    return 0;
  }
  file >> nteams;

  // Kashiwabara HAPの読み込み
  HomeAwayPatternSets hap(nteams);
  for (int t = 0; t < nteams; ++t) {
    for (int s = 0; s < nteams-1; ++s) {
      int ha;
      file >> ha;
      hap.set(t, s, (HomeAway)ha);
    }
  }
  file.close();

  hap.print_mode = kONELINE;
  std::cout << nteams << ' ' << nteams-1 << ' ' << hap << std::endl;

  return 0;
}
