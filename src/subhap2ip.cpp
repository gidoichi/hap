#include <stdio.h>

int main(int argc, char** argv) {
  int nteams, nslots; // チーム数，ラウンド数
  scanf("%d %d", &nteams, &nslots);
  int hap[nteams][nslots];
  for (int t = 0; t < nteams; ++t) {
    for (int s = 0; s < nslots; ++s) {
      scanf("%d", &hap[t][s]);
    }
  }

  puts("maximize"); // ===============================================
  for (int i = 0; i < nteams; ++i) {
    for (int j = 0; j < i; ++j) {
      for (int s = 0; s < nslots; ++s) {
        printf(" + x(%d,%d,%d)", i, j, s);
      }
      puts("");
    }
  }

  puts("subject to"); // =============================================
  int c = 0; // condition No.
  for (int i = 0; i < nteams; ++i) {
    for (int j = 0; j < i; ++j) {
      printf("c%d:", ++c);
      for (int s = 0; s < nslots; ++s) {
        printf(" + x(%d,%d,%d)", i, j, s);
      }
      puts(" <= 1");
    }
  }
  for (int i = 0; i < nteams; ++i) {
    for (int s = 0; s < nslots; ++s) {
      printf("c%d:", ++c);
      for (int j = 0; j < i; ++j) {
        printf(" + x(%d,%d,%d)", i, j, s);
      }
      for (int j = i+1; j < nteams; ++j) {
        printf(" + x(%d,%d,%d)", j, i, s);
      }
      puts(" <= 1");
    }
  }
  for (int i = 0; i < nteams; ++i) {
    for (int j = 0; j < i; ++j) {
      for (int s = 0; s < nslots; ++s) {
        if (hap[i][s] == hap[j][s])
          printf("c%d: x(%d,%d,%d) <= 0\n", ++c, i, j, s);
      }
    }
  }

  puts("binary"); // =================================================
  for (int i = 0; i < nteams; ++i) {
    for (int j = 0; j < i; ++j) {
      for (int s = 0; s < nslots; ++s) {
        printf("x(%d,%d,%d)\n", i, j, s);
      }
    }
  }

  puts("end"); // ====================================================

  return 0;
}
