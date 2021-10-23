#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

struct Edge {
  int to;
};
using Graph = vector<vector<Edge>>;

struct Task {
  using NV = tuple<bool, Int, Int>;  // Node Value
  using EV = NV;  // Edge Value
  vector<char> is_d;

  explicit Task(vector<char> is_d) : is_d(move(is_d)) {}

  EV id() const { return {false, 0, 0}; }

  NV add_node(const EV &ev, int v) const {
    auto[has_d, roundtrip, oneway] = ev;
    if (not has_d) {
      return {is_d[v], 0, 0};
    }
    return ev;
  }

  EV add_edge(const NV &nv, const Edge &e) const {
    auto[has_d, roundtrip, oneway] = nv;
    if (not has_d) return nv;
    return {true, roundtrip + 2, oneway + 1};
  }

  EV merge(const EV &ev1, const EV &ev2) const {
    auto[has_d1, roundtrip1, oneway1] = ev1;
    auto[has_d2, roundtrip2, oneway2] = ev2;
    if (not has_d1) return ev2;
    if (not has_d2) return ev1;
    return {true, roundtrip1 + roundtrip2, min(roundtrip1 + oneway2, roundtrip2 + oneway1)};
  }
};

template<typename Rerootable = Task>
class Rerooter {
 private:
  using NV = typename Rerootable::NV;
  using EV = typename Rerootable::EV;

  Rerootable task;
  int n;                             // number of nodes
  std::vector<std::vector<Edge>> g;  // graph (tree)
  std::vector<NV> sub;               // values for each subtree rooted at i
  std::vector<NV> full;              // values for each entire tree rooted at i
  int base_root;                     // base root node where we start DFS

 public:
  explicit Rerooter(Rerootable task, std::vector<std::vector<Edge>> g,
                    int r = 0)
      : task(move(task)),
        n((int) g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, std::nullopt);
    return full;
  }

 private:
  NV pull_up(int v, int par) {
    EV res = task.id();
    for (auto &e: g[v]) {
      int u = e.to;
      if (u == par) continue;
      auto sub = pull_up(u, v);
      res = task.merge(res, task.add_edge(std::move(sub), e));
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, std::optional<NV> upper_sub) {
    int m = g[v].size();
    std::vector<EV> cuml(m + 1, task.id()), cumr(m + 1, task.id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        assert(upper_sub.has_value());
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(*upper_sub, e));
      } else {
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(sub[u], e));
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cumr[i] = task.merge(task.add_edge(*upper_sub, e), cumr[i + 1]);
      } else {
        cumr[i] = task.merge(task.add_edge(sub[u], e), cumr[i + 1]);
      }
    }

    full[v] = task.add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      std::optional<NV> next_upper_sub{
          task.add_node(task.merge(cuml[i], cumr[i + 1]), v)};
      push_down(u, v, std::move(next_upper_sub));
    }
  }
};

auto solve() {
  int n = in, K = in;
  Graph g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back({v});
    g[v].push_back({u});
  }
  vector<char> D(n);
  REP(i, K) {
    int d = in;
    --d;
    D[d] = true;
  }
  Task task(D);
  Rerooter<> rerooter(task, g);
  auto res = rerooter.run();
  REP(v, n) {
    print(get<2>(res[v]));
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
