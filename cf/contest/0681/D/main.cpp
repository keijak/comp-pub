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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
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

optional<deque<int>> solve() {
  int n, m;
  cin >> n >> m;
  V<int> par(n, -1);
  V<set<int>> child(n);
  REP(i, m) {
    int p, q;
    cin >> p >> q;
    --p;
    --q;
    par[q] = p;
    child[p].insert(q);
  }
  V<int> wish(n);
  REP(i, n) {
    cin >> wish[i];
    --wish[i];
  }

  V<int> roots;
  REP(i, n) {
    if (par[i] == -1) {
      roots.push_back(i);
    }
  }

  set<int> ress;
  deque<int> res;
  auto dfs = [&](auto self, int v) -> bool {
    if (wish[v] != v and not(par[v] >= 0 and wish[v] == wish[par[v]])) {
      return false;
    }
    if (not ress.count(wish[v])) {
      ress.insert(wish[v]);
      res.push_front(wish[v]);
    }
    for (auto c : child[v]) {
      if (not self(self, c)) return false;
    }
    return true;
  };
  for (auto r : roots) {
    assert(wish[r] == r);
    if (not dfs(dfs, r)) return nullopt;
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  auto res = solve();
  if (not res.has_value()) {
    cout << -1 << '\n';
    return 0;
  }

  cout << res.value().size() << '\n';
  for (auto v : res.value()) {
    cout << (v + 1) << '\n';
  }
}
