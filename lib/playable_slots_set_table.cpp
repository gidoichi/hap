#include <stdio.h>
#include "playable_slots_set_table.h"

PlayableSlotsSetTable::PlayableSlotsSetTable(HomeAwayPatternSets &hap)
  : nteams(hap.nteams), hap(hap)
{
  // 表の作成
  table = new std::set<int>*[nteams];
  for (int i = 0; i < nteams; ++i)
    table[i] = new std::set<int>[nteams];


  // 要素の挿入
  for (int i = 0; i < nteams; ++i) {
    table[i][i].insert(nteams);
    for (int j = i + 1; j < nteams; ++j) {
      for (int s = 0; s < nteams-1; ++s) {
        if (hap.get(i, s) != hap.get(j, s))
          table[i][j].insert(s);
      }
    }
  }
}

PlayableSlotsSetTable::~PlayableSlotsSetTable()
{
  for (int i = 0; i < nteams; ++i) {
    delete[] table[i];
  }
  delete[] table;
}

std::set<int> PlayableSlotsSetTable::get(int t1, int t2) const
{
  if (t1 > t2) {
    int t = t1; t1 = t2; t2 = t;
  }
  return table[t1][t2];
}

void PlayableSlotsSetTable::print() const
{
  print(0);
}

void PlayableSlotsSetTable::print(int mode) const
{
  switch(mode) {
  case 0:
    for (int i = 0; i < nteams; ++i) {
      for (int j = i+1; j < nteams; ++j) {
        printf("(%d, %d): ", i+1, j+1);
        for (int slot: get(i, j)) {
          printf("%d ", slot+1);
        }
        puts("");
      }
    }
  }
}
