#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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

struct Book {
  int c;
  vector<int> a;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, X;
  cin >> N >> M >> X;
  vector<Book> books(N);
  REP(i, N) {
    cin >> books[i].c;
    books[i].a.resize(M);
    REP(j, M) { cin >> books[i].a[j]; }
  }
  const int L = 1 << N;
  const i64 INF = 1LL << 40;
  i64 mincost = INF;
  for (int i = 0; i < L; ++i) {
    i64 cost = 0;
    vector<i64> xs(M, X);
    REP(j, N) {
      if (i & (1 << j)) {
        cost += books[j].c;
        REP(k, M) { xs[k] -= books[j].a[k]; }
      }
    }
    if (cost >= mincost) continue;
    bool ok = true;
    REP(k, M) {
      if (xs[k] > 0) {
        ok = false;
        break;
      }
    }
    if (ok) {
      mincost = cost;
    }
  }
  cout << (mincost == INF ? -1LL : mincost) << endl;
}