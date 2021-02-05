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

#ifdef ENABLE_DEBUG
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

optional<vector<int>> solve() {
  int n, m;
  cin >> n >> m;
  // initial color, desired color, painter's color
  vector<int> a(n), b(n), c(m);
  cin >> a >> b >> c;
  map<int, vector<int>> tasks;
  map<int, int> same_planks;
  REP(i, n) {
    if (a[i] == b[i]) {
      same_planks[a[i]] = i;
      continue;
    }
    tasks[b[i]].push_back(i);
  }
  map<int, vector<int>> painter;
  REP(i, m) { painter[c[i]].push_back(i); }
  vector<int> ans(m, -1);
  int last_valid_painter = -1;
  int last_plank = -1;
  for (const auto &[c, planks] : tasks) {
    const auto &ps = painter[c];
    if (ssize(planks) > ssize(ps)) {
      return nullopt;
    }
    int i = 0;
    while (i < ssize(ps)) {
      int p = ps[i];
      ans[p] = (i < ssize(planks)) ? (planks[i] + 1) : (planks[0] + 1);
      if (chmax(last_valid_painter, p)) {
        last_plank = ans[p];
      }
      ++i;
    }
  }
  for (int i = m - 1; i >= 0; --i) {
    if (ans[i] == -1) {
      auto it = same_planks.find(c[i]);
      if (it != same_planks.end()) {
        ans[i] = it->second + 1;
        if (chmax(last_valid_painter, i)) {
          last_plank = ans[i];
        }
        continue;
      }
      if (i > last_valid_painter) {
        return nullopt;
      }
      if (last_plank == -1) return nullopt;
      ans[i] = last_plank;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    auto res = solve();
    if (!res) {
      cout << "NO\n";
    } else {
      cout << "YES\n";
      print_seq(res.value());
    }
  }
}
