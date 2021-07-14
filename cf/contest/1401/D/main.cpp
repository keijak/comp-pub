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
  return pprint(a, ",", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ",", "", &(os << "{")) << "}";
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

const i64 MOD = 1'000'000'007;

struct mint {
  long long x;
  mint(long long x = 0) : x((x % MOD + MOD) % MOD) {}
  mint operator-() const { return mint(-x); }
  mint &operator+=(const mint &a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint &operator-=(const mint &a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint &operator*=(const mint &a) {
    (x *= a.x) %= MOD;
    return *this;
  }
  mint pow(long long t) const {
    if (!t) return 1;
    mint a = pow(t >> 1);
    a *= a;
    if (t & 1) a *= *this;
    return a;
  }
  // for prime MOD
  mint inv() const { return pow(MOD - 2); }
  mint &operator/=(const mint &a) { return *this *= a.inv(); }
};
mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
bool operator<(const mint &a, const mint &b) { return a.x < b.x; }
bool operator>(const mint &a, const mint &b) { return a.x > b.x; }
bool operator<=(const mint &a, const mint &b) { return a.x <= b.x; }
bool operator>=(const mint &a, const mint &b) { return a.x >= b.x; }
istream &operator>>(istream &is, mint &a) { return is >> a.x; }
ostream &operator<<(ostream &os, const mint &a) { return os << a.x; }

mint solve() {
  int n;
  cin >> n;
  V<V<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  int m;  // the number of prime factors of k.
  cin >> m;
  V<int> primes(m);  // k = product(primes);
  REP(i, m) cin >> primes[i];
  sort(primes.rbegin(), primes.rend());

  V<int> sizes(n);
  auto dfs1 = [&](auto self, int v, int p) -> int {
    int res = 1;
    for (auto u : g[v]) {
      if (u != p) {
        res += self(self, u, v);
      }
    }
    return sizes[v] = res;
  };
  dfs1(dfs1, 0, -1);

  V<i64> contrib;
  contrib.reserve(n);
  auto dfs2 = [&](auto self, int v, int p) -> void {
    for (auto u : g[v]) {
      if (u != p) {
        i64 up_count = n - sizes[u];
        i64 down_count = sizes[u];
        contrib.push_back(up_count * down_count);
        self(self, u, v);
      }
    }
  };
  dfs2(dfs2, 0, -1);

  assert(int(contrib.size()) == n - 1);
  sort(contrib.rbegin(), contrib.rend());

  mint ans = 0;
  if (m <= n - 1) {
    REP(i, m) { ans += mint(contrib[i]) * primes[i]; }
    for (int i = m; i < n - 1; ++i) {
      ans += contrib[i];
    }
  } else {
    mint pp = 1;
    int r = m - n + 2;
    REP(i, r) { pp *= mint(primes[i]); }
    ans = mint(contrib[0]) * pp;
    for (int i = 1; i < n - 1; ++i) {
      ans += mint(contrib[i]) * primes[r + i - 1];
    }
  }

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
