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
  char *dir = new char[255];
  getcwd(dir, 255);
  std::ifstream file(std::string(dir) + "/data/Infeasible_HAP_set_of_Kashiwabara.txt", std::ifstream::in);
  if (!file.is_open()) {
    puts("File open error.");
    return 0;
  }
  int n;
  file >> n;
  HomeAwayPatternSets hap(n);
  for (int t = 0; t < n; ++t) {
    for (int s = 0; s < n-1; ++s) {
      static HomeAway hapint[] = {kAway, kHome};
      int ha;
      file >> ha;
      hap.set(t, s, hapint[ha]);
    }
  }
  file.close();
  hap.print();

  PlayableSlotsSetTable pst(hap);
  puts("");
  pst.print();

  return 0;
}
