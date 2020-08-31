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
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

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

const i64 LINF = 1e18;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int K;
  cin >> K;
  string S;
  cin >> S;
  int N = 1 << (2 * K);

  V<V<int>> st(3, V<int>(N + 1));
  REP(i, N) {
    REP(j, 3) { st[j][i + 1] = st[j][i]; }

    char ch = S[i];
    if (ch == 'J') {
      st[0][i + 1]++;
    } else if (ch == 'O') {
      st[1][i + 1]++;
    } else if (ch == 'I') {
      st[2][i + 1]++;
    }
  }

  auto range_count = [&](int l, int r, int x) {
    if (l <= r) {
      return st[x][r] - st[x][l];
    } else {
      return st[x][r] + (st[x][N] - st[x][l]);
    }
  };

  auto calc = [&](auto self, int l, int r, int k) -> int {
    if (k == 0) return 0;
    int width = 1 << (2 * (k - 1));
    int j1 = (l + width) % N;
    int j2 = (j1 + width) % N;
    int j3 = (j2 + width) % N;
    int c1 = width - range_count(l, j1, 0);
    int c2 = width - range_count(j1, j2, 1);
    int c3 = width - range_count(j2, j3, 2);
    int c4 = self(self, j3, r, k - 1);
    return c1 + c2 + c3 + c4;
  };

  i64 ans = LINF;
  REP(i, N) {
    i64 cost = calc(calc, i, i, K);
    ans = min(ans, cost);
    DEBUG(i, cost, ans);
  }
  cout << ans << endl;
}
