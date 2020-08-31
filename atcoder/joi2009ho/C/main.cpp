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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, H, K;
  cin >> N >> M >> H >> K;
  V<int> S(N);
  cin >> S;
  V<tuple<int, int>> swaps(M);
  REP(i, M) {
    int a, b;
    cin >> a >> b;
    swaps[i] = {b, a};
  }
  sort(swaps.begin(), swaps.end());

  // Simulation 1.
  V<int> state(N);
  REP(i, N) state[i] = i;
  REP(i, M) {
    auto [_, a] = swaps[i];
    swap(state[a - 1], state[a]);
  }

  V<int> rev(N);
  REP(i, N) { rev[state[i]] = i; }

  int score = 0;
  REP(i, K) { score += S[rev[i]]; }

  // Simulation 2.
  int max_reduce = 0;
  V<int> state2(N);
  REP(i, N) state2[i] = i;
  REP(i, M) {
    auto [_, a] = swaps[i];
    int p1 = min(state2[a - 1], state2[a]);
    int p2 = max(state2[a - 1], state2[a]);
    if (p1 < K and p2 >= K) {
      if (S[rev[p1]] > S[rev[p2]]) {
        chmax(max_reduce, S[rev[p1]] - S[rev[p2]]);
      }
    }
    swap(state2[a - 1], state2[a]);
  }

  int ans = score - max_reduce;
  DEBUG(score, max_reduce, ans);
  cout << ans << endl;
}
