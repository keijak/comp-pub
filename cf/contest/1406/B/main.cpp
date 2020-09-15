#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define WHOLE(x) std::begin(x), std::end(x)
#define LEN(a) int((a).size())

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

const i64 INF = 1e18;

i64 solve() {
  int n;
  cin >> n;
  V<i64> a(n);
  cin >> a;

  auto dpp = make_vec(n + 1, 6, -INF);
  auto dpn = make_vec(n + 1, 6, INF);
  REP(i, n + 1) { dpp[i][0] = dpn[i][0] = 1; }
  // REP(i, n) { dpp[i + 1][1] = a[i]; }

  for (int i = 1; i <= n; ++i) {
    i64 x = a[i - 1];
    for (int j = 1; j <= 5; ++j) {
      chmax(dpp[i][j], dpp[i - 1][j]);
      if (dpp[i - 1][j - 1] != -INF) {
        chmax(dpp[i][j], dpp[i - 1][j - 1] * x);
        chmin(dpn[i][j], dpp[i - 1][j - 1] * x);
      }
      chmin(dpn[i][j], dpn[i - 1][j]);
      if (dpn[i - 1][j - 1] != INF) {
        chmax(dpp[i][j], dpn[i - 1][j - 1] * x);
        chmin(dpn[i][j], dpn[i - 1][j - 1] * x);
      }
    }
  }
  i64 ans = a[0] * a[1] * a[2] * a[3] * a[4];
  chmax(ans, dpp[n][5]);
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << solve() << '\n';
  }
}
