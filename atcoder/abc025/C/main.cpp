#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const i64 INF = 1LL << 40;

int B[2][3];
int C[3][2];

struct Board {
  Board() {
    REP(i, 3) REP(j, 3) { board[i][j] = 0; }
  }
  Board(const Board& b) {
    REP(i, 3) REP(j, 3) { this->board[i][j] = b.board[i][j]; }
  }
  Board(Board&& b) { swap(this->board, b.board); }

  int score() {
    int res = 0;
    REP(i, 2) REP(j, 3) {
      if (board[i][j] == board[i + 1][j]) {
        res += B[i][j];
      }
    }
    REP(i, 3) REP(j, 2) {
      if (board[i][j] == board[i][j + 1]) {
        res += C[i][j];
      }
    }
    return res;
  }

  int board[3][3];
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 total_score = 0;
  REP(i, 2) REP(j, 3) {
    cin >> B[i][j];
    total_score += B[i][j];
  }
  REP(i, 3) REP(j, 2) {
    cin >> C[i][j];
    total_score += C[i][j];
  }

  function<i64(int, Board)> maximize, minimize;

  maximize = [&](int k, Board b) -> i64 {
    if (k == 9) return b.score();
    i64 res = -INF;
    REP(i, 3) REP(j, 3) {
      if (b.board[i][j] != 0) continue;
      b.board[i][j] = 1;
      i64 minval = minimize(k + 1, b);
      res = max(res, minval);
      b.board[i][j] = 0;
    }
    return res;
  };

  minimize = [&](int k, Board b) -> i64 {
    if (k == 9) return b.score();
    i64 res = INF;
    REP(i, 3) REP(j, 3) {
      if (b.board[i][j] != 0) continue;
      b.board[i][j] = -1;
      i64 maxval = maximize(k + 1, b);
      res = min(res, maxval);
      b.board[i][j] = 0;
    }
    return res;
  };

  i64 score1 = maximize(0, Board{});
  i64 score2 = total_score - score1;
  cout << score1 << endl << score2 << endl;
}
