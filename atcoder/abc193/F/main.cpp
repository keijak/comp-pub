#include <bits/stdc++.h>

#include <atcoder/maxflow>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

template <typename T>
struct ProjectSelection {
 private:
  const int BIG_COST = 10000;

  // Assign Color 1 to kSource and Color 0 to kSink.
  const int kSource, kSink;
  const int n_;  // number of initial nodes
  int m_;        // number of additional nodes

  struct Edge {
    int from;
    int to;
    T cost;
  };
  vector<Edge> edges_;

  T bonus_;

 public:
  explicit ProjectSelection(int n)
      : kSource(n), kSink(n + 1), n_(n), m_(0), bonus_(0) {}

  // {X=1, Y=0} => penalty
  void X_and_notY_lose(int x, int y, T penalty) {
    edges_.push_back(Edge{x, y, penalty});
  }

  // {X=1} => penalty
  void X_lose(int x, T penalty) { X_and_notY_lose(x, kSink, penalty); }

  // {X=0} => penalty
  void notX_lose(int x, T penalty) { X_and_notY_lose(kSource, x, penalty); }

  // {X=1} => revenue
  void X_gain(int x, T revenue) {
    bonus_ += revenue;
    notX_lose(x, revenue);
  }

  // {X=0} => revenue
  void notX_gain(int x, T revenue) {
    bonus_ += revenue;
    X_lose(x, revenue);
  }

  // {X=0,Y=1} or {X=1,Y=0} => penalty
  void X_xor_Y_lose(int x, int y, T penalty) {
    X_and_notY_lose(x, y, penalty);
    X_and_notY_lose(y, x, penalty);
  }

  // {Xi=1 forall i} => revenue
  void all_of_X_gain(vector<int> x, T revenue) {
    const int y = n_ + 2 + m_++;
    bonus_ += revenue;
    X_and_notY_lose(kSource, y, revenue);
    for (const auto &v : x) {
      X_and_notY_lose(y, v, BIG_COST);
    }
  }

  // {Xi=0 forall i} => revenue
  void none_of_X_gain(vector<int> x, T revenue) {
    const int y = n_ + 2 + m_++;
    bonus_ += revenue;
    X_and_notY_lose(y, kSink, revenue);
    for (const auto &v : x) {
      X_and_notY_lose(v, y, BIG_COST);
    }
  }

  // {X=0,Y=0} or {X=1,Y=1} => revenue
  void X_equal_Y_gain(int x, int y, T revenue) {
    all_of_X_gain({x, y}, revenue);
    none_of_X_gain({x, y}, revenue);
  }

  T min_cost() const {
    atcoder::mf_graph<T> g(n_ + m_ + 2);
    for (const Edge &e : edges_) {
      g.add_edge(e.from, e.to, e.cost);
    }
    return g.flow(kSource, kSink);
  }

  T max_revenue() const { return bonus_ - min_cost(); }
};

i64 solve() {
  int n;
  cin >> n;
  vector<string> g(n);
  vector<vector<int>> node_id(n, vector<int>(n, -1));
  int m = 0;
  REP(i, n) {
    string s;
    cin >> s;
    assert(ssize(s) == n);
    REP(j, n) {
      if ((i + j) % 2 == 0) {
        if (s[j] == 'B') {
          s[j] = 'W';
        } else if (s[j] == 'W') {
          s[j] = 'B';
        }
      }
      if (s[j] == '?') {
        node_id[i][j] = m++;
      }
    }
    g[i] = s;
  }

  ProjectSelection<int> psp(m);
  i64 ans = 0;

  REP(rot, 2) {
    REP(i, n) {
      REP(j, n - 1) {
        const char x = g[i][j], y = g[i][j + 1];
        const int u = node_id[i][j], v = node_id[i][j + 1];
        if (x != '?' and y != '?') {
          if (x == y) ++ans;
        } else if (x == '?' and y == '?') {
          psp.X_equal_Y_gain(u, v, 1);
        } else if (x == '?') {
          if (y == 'B') {
            psp.notX_gain(u, 1);
          } else {
            psp.X_gain(u, 1);
          }
        } else if (y == '?') {
          if (x == 'B') {
            psp.notX_gain(v, 1);
          } else {
            psp.X_gain(v, 1);
          }
        } else {
          assert(false);
        }
      }
    }

    vector<string> g2 = g;
    vector<vector<int>> ni2 = node_id;
    REP(i, n) REP(j, n) {
      g2[j][i] = g[i][j];
      ni2[j][i] = node_id[i][j];
    }
    swap(g, g2);
    swap(node_id, ni2);
  }
  ans += psp.max_revenue();
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
