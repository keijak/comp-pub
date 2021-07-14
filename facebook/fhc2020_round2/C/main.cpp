#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
void chmax(T &a, T b) {
  if (a < b) a = std::move(b);
}
template <class T>
void chmin(T &a, T b) {
  if (a > b) a = std::move(b);
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

i64 solve() {
  // input
  i64 N, M, E, K;  // < 1e6
  cin >> N >> M >> E >> K;
  V<i64> X(N), Y(N), I(N), W(N);
  i64 AX, BX, CX, AY, BY, CY;
  i64 AI, BI, CI, AW, BW, CW;
  REP(i, K) { cin >> X[i]; }
  cin >> AX >> BX >> CX;
  REP(i, K) { cin >> Y[i]; }
  cin >> AY >> BY >> CY;
  REP(i, K) { cin >> I[i]; }
  cin >> AI >> BI >> CI;
  REP(i, K) { cin >> W[i]; }
  cin >> AW >> BW >> CW;

  const i64 NE = (i64)N * M + N;
  const i64 WMOD = 1'000'000'000;
  for (int i = K; i < N; ++i) {
    X[i] = ((AX * X[i - 2]) % M + (BX * X[i - 1]) % M + CX) % M;
    Y[i] = ((AY * Y[i - 2]) % M + (BY * Y[i - 1]) % M + CY) % M;
    I[i] = ((AI * I[i - 2]) % NE + (BI * I[i - 1]) % NE + CI) % NE;
    W[i] = ((AW * W[i - 2]) % WMOD + (BW * W[i - 1]) % WMOD + CW) % WMOD;
  }

  V<i64> weight(NE, 1);
  multiset<i64> outer_set;
  REP(i, N) outer_set.insert(1);
  V<multiset<i64>> inner_set1(N), inner_set2(N);

  i64 mst_cost = N - 1 + N * (M - 1);
  REP(i, N) {
    int node_id = X[i];
    do {
      inner_set1[i].emplace(1);
      node_id = (node_id + 1) % M;
    } while (node_id != Y[i]);
    while (node_id != X[i]) {
      inner_set2[i].emplace(1);
      node_id = (node_id + 1) % M;
    }
    if (X[i] != Y[i]) {
      outer_set.insert(1);
    }
  }

  for (int i = 0; i < E; ++i) {
    int edge_id = I[i];
    i64 prev_weight = weight[edge_id];
    weight[edge_id] = W[i];
    if (edge_id >= N * M) {
      i64 max_cost = *outer_set.rbegin();
      if (max_cost == prev_weight && max_cost < W[i]) {
        mst_cost -= max_cost;
        mst_cost += W[i];
      }
      auto it = outer_set.find(prev_weight);
      outer_set.erase(it);
      outer_set.insert(W[i]);
    } else {
      int circle_id = edge_id / M;
      int node_id = edge_id % M;
      assert(circle_id < N);
      auto &set1 = inner_set1[circle_id];
      auto &set2 = inner_set2[circle_id];
      if (X[circle_id] == Y[circle_id]) {
        i64 max_cost = *set1.rbegin();
        if (max_cost == prev_weight && max_cost < W[i]) {
          mst_cost -= max_cost;
          mst_cost += W[i];
        }
        auto it = set1.find(prev_weight);
        set1.erase(it);
        set1.insert(W[i]);
      }
    }
  }

  return -1;  // impossible
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  REP(i, T) { cout << "Case #" << (i + 1) << ": " << solve() << '\n'; }
}
