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

// Partially Persistent UnionFind.
struct UnionFindWithTime {
  int n;
  mutable std::vector<int> parent_;  // positive: parent, negative: size
  std::vector<int> rank_;
  int num_roots_;
  int clock_;
  std::vector<int> parented_time_;
  std::vector<std::vector<std::pair<int, int>>> size_history_;

  explicit UnionFindWithTime(int sz)
      : n(sz),
        parent_(sz, -1),
        rank_(sz, 1),
        num_roots_(sz),
        clock_(0),
        parented_time_(sz, -1),
        size_history_(n, {{0, 1}}) {}

  // Returns current clock_.
  int unite(int x, int y) {
    ++clock_;
    x = find(x, clock_), y = find(y, clock_);
    if (x == y) return clock_;
    if (rank_[x] < rank_[y] or
        (rank_[x] == rank_[y] and -parent_[x] < -parent_[y])) {
      std::swap(x, y);
    }
    parent_[x] += parent_[y];
    parent_[y] = x;
    rank_[x] = std::max(rank_[x], rank_[y] + 1);
    parented_time_[y] = clock_;
    size_history_[x].emplace_back(clock_, -parent_[x]);
    --num_roots_;
    return clock_;
  }

  int find(int v, int time) const {
    if (parent_[v] < 0) return v;
    if (time < parented_time_[v]) return v;
    return find(parent_[v], time);
  }
  int find(int v) const { return find(v, clock_); }

  int size(int v, int time) const {
    int r = find(v, time);
    const auto &h = size_history_[r];
    auto it = std::lower_bound(h.begin(), h.end(), std::pair(time + 1, -1));
    return (--it)->second;
  }
  int size(int v) const { return -parent_[find(v)]; }

  bool same(int x, int y, int time) const {
    return find(x, time) == find(y, time);
  }
  bool same(int x, int y) const { return find(x) == find(y); }

  std::optional<int> united_time(int x, int y) {
    if (not same(x, y)) {
      return std::nullopt;
    }
    int fv = 0, tv = clock_;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      if (same(x, y, mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  }
};

void solve() {
  int n, m, q;
  cin >> n >> m >> q;
  set<array<int, 2>> init;
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    init.insert({a, b});
  }

  vector<array<int, 2>> events(q);
  REP(i, q) {
    int c, d;
    cin >> c >> d;
    --c, --d;
    init.erase({c, d});
    events[i] = {c, d};
  }

  UnionFindWithTime uf(n);
  for (auto [a, b] : init) {
    uf.unite(a, b);
  }
  int start_time = uf.clock_;
  REP(i, q) {
    auto [c, d] = events[q - 1 - i];
    uf.unite(c, d);
  }
  int end_time = uf.clock_;

  vector<int> ans(n, -2);
  for (int i = 1; i < n; ++i) {
    if (uf.same(i, 0, start_time)) {
      ans[i] = -1;
      continue;
    }
    auto res = uf.united_time(i, 0);
    if (!res) {
      ans[i] = 0;
    } else {
      ans[i] = end_time - (res.value() - 1);
    }
  }
  REP(i, 1, n) {
    cout << ans[i] << '\n';
    assert(ans[i] != -2);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
