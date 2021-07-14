#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

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
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
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

const int INF = 1e8;

struct Range {
  int sum;
  int l;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  V<int> L(N);
  cin >> L;

  V<int> cum(N + 1), lmin(N + 1, INF), rmin(N + 1, INF), lmax(N + 1, 0),
      rmax(N + 1, 0);
  REP(i, N) {
    cum[i + 1] = cum[i] + L[i];
    lmin[i + 1] = min(lmin[i], L[i]);
    rmin[N - 1 - i] = min(rmin[N - i], L[N - 1 - i]);
    lmax[i + 1] = max(lmax[i], L[i]);
    rmax[N - 1 - i] = max(rmax[N - i], L[N - 1 - i]);
  }

  int ans = lmax[N] - lmin[N];

  auto largest_min = [&](int l, int r, int tmax) -> int {
    int n = r - l;
    auto dp = make_vec(n + 1, INF);
    for (int i = 1; i <= n; ++i) {
      int val = L[l + i - 1];
      dp[i] = min(dp[i - 1], val);
      for (int j = i - 1; j >= 1; --j) {
        int p = L[l + j - 1];
        val += p;
        if (val > tmax) break;
        chmax(dp[i], min(dp[j - 1], val));
      }
    }
    return dp[n];
  };

  for (int l = 0; l < N; ++l) {
    for (int r = l + 1; r <= N; ++r) {
      if (l == 0 and r == N) continue;
      int tmax = cum[r] - cum[l];
      if (tmax < lmax[N]) continue;

      int tmin = INF;
      if (l > 0) {
        int m = largest_min(0, l, tmax);
        if (0 < m and m <= tmax) chmin(tmin, m);
      }
      if (r < N) {
        int m = largest_min(r, N, tmax);
        if (0 < m and m <= tmax) chmin(tmin, m);
      }
      if (0 < tmin and tmin <= tmax) {
        chmin(ans, tmax - tmin);
      }
    }
  }
  cout << ans << endl;
}
