#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<V<int>> adj(n);
  REP(i, m) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    adj[u].push_back(v);
  }

  const int INF = 1e9;
  int min_cycle = INF;
  V<int> ans;
  ans.reserve(n);
  REP(r, n) {
    deque<tuple<int, int>> q;
    V<optional<int>> dist(n);
    V<int> par(n, -1);
    for (auto v : adj[r]) {
      dist[v] = 1;
      par[v] = r;
      q.emplace_back(v, 1);
    }
    while (q.size()) {
      auto [cur, d] = q.front();
      q.pop_front();
      if (d >= min_cycle) break;
      if (cur == r) {
        if (chmin(min_cycle, d)) {
          ans.clear();
          ans.push_back(r);
          for (int v = par[r]; v != r; v = par[v]) {
            ans.push_back(v);
          }
        }
        break;
      }
      for (auto v : adj[cur]) {
        if (dist[v].has_value()) continue;
        dist[v] = d + 1;
        par[v] = cur;
        q.emplace_back(v, d + 1);
      }
    }
  }

  if (min_cycle == INF) {
    cout << -1 << endl;
  } else {
    cout << ans.size() << '\n';
    for (auto z : ans) cout << z + 1 << '\n';
  }
}
