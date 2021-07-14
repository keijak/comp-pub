#include <bits/stdc++.h>

#include <boost/rational.hpp>
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

using boost::rational;
using Memo = V<V<map<tuple<int, int>, int>>>;

const int INF = 1e8;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  V<int> L(N);
  cin >> L;
  int lmin = INF, lmax = -INF;
  V<int> cum(N + 1);
  REP(i, N) {
    cum[i + 1] = cum[i] + L[i];
    chmin(lmin, L[i]);
    chmax(lmax, L[i]);
  }
  assert(lmin <= lmax);

  auto solve = [&](auto self, Memo &memo, const rational<int> mean, int i,
                   int k, int tmin, int tmax) -> int {
    assert(k >= 1);
    if (k == 1 && i < N) {
      int val = cum[N] - cum[i];
      assert(val > 0);
      chmin(tmin, val);
      chmax(tmax, val);
      return tmax - tmin;
    }
    if (N - i < k) {
      return INF;
    }
    tuple<int, int> key = {tmin, tmax};
    auto &memo_ik = memo[i][k];
    if (auto it = memo_ik.find(key); it != memo_ik.end()) {
      return it->second;
    }

    int val = 0;
    int ans = INF;
    for (int j = i; j < N; ++j) {
      val += L[j];
      // if (j + 1 < N and val + L[j + 1] <= mean - 40) {
      //   continue;
      // }
      int r =
          self(self, memo, mean, j + 1, k - 1, min(tmin, val), max(tmax, val));
      chmin(ans, r);
      // if (val >= mean + 40) break;
    }
    return (memo_ik[key] = ans);
  };

  int ans = lmax - lmin;
  for (int k = 2; k < N; ++k) {
    rational<int> mean(cum[N], k);
    Memo memo = make_vec(N, N, map<tuple<int, int>, int>());
    int r = solve(solve, memo, mean, 0, k, INF, -INF);
    if (chmin(ans, r)) {
      DEBUG(r, k);
    }
  }
  cout << ans << endl;
}
