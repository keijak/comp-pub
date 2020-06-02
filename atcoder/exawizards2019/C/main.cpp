#include <iostream>
#include <string>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

const int QMAX = 200000;
char T[QMAX], D[QMAX];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  cin.ignore();
  string board;
  getline(cin, board);
  REP(i, Q) { cin >> T[i] >> D[i]; }
  int head = -1, tail = N;
  for (int i = Q - 1; i >= 0; --i) {
    char t = T[i];
    if (D[i] == 'L') {
      if (head + 1 < N && t == board[head + 1]) ++head;
      if (tail < N && t == board[tail]) ++tail;
    } else {
      if (tail - 1 >= 0 && t == board[tail - 1]) --tail;
      if (head >= 0 && t == board[head]) --head;
    }
  }
  cout << (tail - head - 1) << endl;
}
