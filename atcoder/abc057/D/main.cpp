#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}

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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << fixed << setprecision(6);

  int N, A, B;
  cin >> N >> A >> B;
  V<i64> v(N);
  REP(i, N) cin >> v[i];
  sort(v.rbegin(), v.rend());

  int rfirst = -1;
  int rcount = 0;
  int maxcount = 0;
  REP(i, N) {
    if (v[i] == v[0]) ++maxcount;
    if (v[i] == v[A - 1]) {
      ++rcount;
      if (rfirst < 0) rfirst = i;
    }
  }

  u64 vsum = 0;
  REP(i, A) { vsum += v[i]; }
  double vmean = (double)vsum / A;

  V<V<u64>> C = make_vec(N + 1, N + 1, 0ULL);
  REP(i, N + 1) C[i][0] = C[i][i] = 1;
  for (int i = 1; i <= N; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }

  DEBUG(vmean, rcount);
  if (v[A - 1] == v[0]) {
    int m = min(maxcount, B);
    u64 cnt = 0;
    for (int k = A; k <= m; ++k) {
      cnt += C[maxcount][k];
      DEBUG(m, k, C[maxcount][k], cnt);
    }
    cout << vmean << '\n' << cnt << '\n';
  } else {
    int k = A - rfirst;
    u64 cnt = C[rcount][k];
    cout << vmean << '\n' << cnt << '\n';
  }
}
