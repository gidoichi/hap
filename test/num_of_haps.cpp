#include <stdio.h>
#include <vector>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include "../lib/home_away_pattern_sets.h"

typedef boost::multiprecision::cpp_int t;

// M[l][r]: スロット数r，チーム数lのときの列・行の並び替えを行ったHAP総数
std::vector<std::vector<t>> M;
t fM(int l, int r); // = M[l][r]

int main(int argc, char* argv[])
{
  int nteams = atoi(argv[1]);
  int nslots = nteams - 1;
  M.resize(nteams+1);
  for (int l = 0; l <= nteams; ++l) {
    M[l].resize(nslots+1);
    for (int r = 0; r <= nslots; ++r) {
      M[l][r] = -1;
    }
  }

  std::cout << fM(nteams, nslots) << std::endl;

  return 0;
}

// MはDPで計算
// 未計算時は-1
t fM(int l, int r)
{
  if (M[l][r] != -1) return M[l][r];

  if (l <= 0) return (M[l][r] = 1);

  t sum = 0;
  for (int k = 0; k <= r; ++k)
    sum += fM(l-1, k) * fM(l-1, r-k);
  return (M[l][r] = sum);
}
