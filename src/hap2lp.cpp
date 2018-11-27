#include <stdio.h>

int main(int argc, char** argv) {
  int n; // チーム数
  scanf("%d", &n);
  int hap[n][n-1];
  for (int t = 0; t < n; ++t) {
    for (int s = 0; s < n-1; ++s) {
      scanf("%d", &hap[t][s]);
    }
  }

  puts("maximize"); // ===============================================
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      for (int s = 0; s < n-1; ++s) {
        printf(" + x(%d,%d,%d)", i, j, s);
      }
      puts("");
    }
  }

  puts("subject to"); // =============================================
  int c = 0; // condition No.
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      printf("c%d:", ++c);
      for (int s = 0; s < n-1; ++s) {
        printf(" + x(%d,%d,%d)", i, j, s);
      }
      puts(" <= 1");
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int s = 0; s < n-1; ++s) {
      printf("c%d:", ++c);
      for (int j = 0; j < i; ++j) {
        printf(" + x(%d,%d,%d)", i, j, s);
      }
      for (int j = i+1; j < n; ++j) {
        printf(" + x(%d,%d,%d)", j, i, s);
      }
      puts(" <= 1");
    }
  }
  puts("bounds"); // =================================================
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      for (int s = 0; s < n-1; ++s) {
        if (hap[i][s] == hap[j][s])
          printf("x(%d,%d,%d) <= 0\n", i, j, s);
        else
          printf("x(%d,%d,%d) <= 1\n", i, j, s);
      }
    }
  }

  puts("end"); // ====================================================

  return 0;
}
