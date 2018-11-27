/**
 * 指定したチーム数のPSMBを出力する
 * @param argv[1] チーム数（偶数）
 * @param argv[2]... break位置（2日目が1）
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool** make_psmb(int n, bool* breaks, bool** table);
void print_psmb(bool** table, size_t size);

int main(int argc, char** argv) {
  int n = atoi(argv[1]);
  bool* breaks = (bool*)calloc(n, sizeof(bool));
  bool** table = (bool**)malloc(n * sizeof(bool*));
  for (int i = 0; i < n; i++) table[i] = (bool*)malloc((n-1) * sizeof(bool));

  // break位置の決定
  for (int i = 1; i <= n/2-1; i++) {
    breaks[atoi(argv[i+1])] = true;
  }

  // 表の作成・出力
  make_psmb(n, breaks, table);
  print_psmb(table, n);
}

bool** make_psmb(int n, bool* breaks, bool** table) {
  int* b = (int*)calloc(n, sizeof(int));
  for (int i = 1; i <= n-2; i++) b[i] = b[i-1] + breaks[i];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n-1; j++) {
      if (j % 2 == 0) {
        table[i][j] = ((i-b[j]+n)%n >= n/2) ? 0 : 1;
      } else {
        table[i][j] = ((i-b[j]+n)%n >= n/2) ? 1 : 0;
      }
    }
  }
  return table;
}

void print_psmb(bool** table, size_t size) {
  char H = '1', A = '0';
  int n = size;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n-1; j++) {
      if (j >= 1 && table[i][j-1] == table[i][j]) {
        // printf("\x1b[4m%c\x1b[0m ", (table[i][j] == 1) ? H : A); // break箇所に下線
        printf("%c ", (table[i][j] == 1) ? H : A);
      } else {
        printf("%c ", (table[i][j] == 1) ? H : A);
      }
    }
    puts("");
  }
  puts("");
}
