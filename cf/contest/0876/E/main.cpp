#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

// Returns the topogically sorted list of nodes.
// Nodes with zero indegree come first.
// The input graph must be a DAG. Otherwise try SCC decomposition.
optional<vector<int>> topological_sort(const vector<vector<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u : g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) q.emplace_back(v);
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u : g[v]) {
      if (--indeg[u] == 0) q.push_back(u);
    }
  }
  if (int(res.size()) != n) return nullopt;  // not a DAG
  return res;
}

optional<vector<int>> solve() {
  int n, m;
  cin >> n >> m;
  vector<deque<int>> words(n);
  REP(i, n) {
    int l;
    cin >> l;
    words[i].resize(l);
    REP(j, l) cin >> words[i][j];
  }
  DEBUG(words);

  vector<vector<int>> g(m + 1);

  auto check_prefix = [&](const deque<int> &x, const deque<int> &y) -> bool {
    if (ssize(x) > ssize(y)) return false;
    REP(i, ssize(x)) {
      if (x[i] != y[i]) return false;
    }
    return true;
  };

  REP(i, n - 1) {
    if (check_prefix(words[i], words[i + 1])) continue;
    if (check_prefix(words[i + 1], words[i])) return nullopt;
    int l = min(ssize(words[i]), ssize(words[i + 1]));
    int pos = -1;
    REP(j, l) {
      if (words[i][j] != words[i + 1][j]) {
        pos = j;
        break;
      }
    }
    assert(pos != -1);
    int x = words[i][pos];
    int y = words[i + 1][pos];
    g[x].push_back(y);  // x <= y
    DEBUG(x, y);
  }

  auto res = topological_sort(g);
  if (!res) return nullopt;
  vector<bool> capit(m + 1, false);
  auto &v = res.value();
  reverse(ALL(v));
  for (auto x : v) {
    bool cap = false;
    for (auto y : g[x]) {
      if (capit[y]) {
        if (x > y) return nullopt;
        cap = true;
      } else if (y < x) {
        cap = true;
      }
    }
    capit[x] = cap;
  }
  vector<int> ans;
  REP(x, 1, m + 1) {
    if (capit[x]) ans.push_back(x);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (!res) {
    cout << "No\n";
  } else {
    cout << "Yes\n";
    auto vals = move(*res);
    cout << ssize(vals) << "\n";
    if (not vals.empty()) print_seq(vals);
  }
}
