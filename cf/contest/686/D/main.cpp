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
struct is_counterable : std::false_type {};
template <typename T>
struct is_counterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                     decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_counterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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

struct HLD {
  int n;
  V<V<int>> child;
  V<int> sizes, tour_in, tour_out, comp_root;
  int counter;

  explicit HLD(V<V<int>> g)
      : n(ssize(g)),
        child(g),
        sizes(n),
        tour_in(n),
        tour_out(n),
        comp_root(n),
        counter(0) {
    dfs_sz();
    dfs_hld();
  }

 private:
  void dfs_sz(int v = 0) {
    sizes[v] = 1;
    for (auto &u : child[v]) {
      dfs_sz(u);
      sizes[v] += sizes[u];
      if (sizes[u] > sizes[child[v][0]]) {
        swap(u, child[v][0]);
      }
    }
  }

  void dfs_hld(int v = 0) {
    tour_in[v] = counter++;
    for (auto u : child[v]) {
      comp_root[u] = (u == child[v][0] ? comp_root[v] : u);
      dfs_hld(u);
    }
    tour_out[v] = counter;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<int> par(N, -1);
  V<V<int>> child(N);
  REP(i, N - 1) {
    int p;
    cin >> p;
    --p;
    par[i + 1] = p;
    child[p].emplace_back(i + 1);
  }
  HLD hld(child);
  DEBUG(hld.tour_in);
  DEBUG(hld.tour_out);
  DEBUG(hld.comp_root);
  V<pair<int, int>> compo(N);
  V<V<int>> compo_node(N);
  V<V<int>> subsizes(N);

  auto dfs_compo = [&](auto self, int v) -> void {
    int cr = hld.comp_root[v];
    int cid = subsizes[cr].size();
    compo[v] = {cr, cid};
    compo_node[cr].push_back(v);
    subsizes[cr].push_back(hld.sizes[v]);
    assert(compo_node[cr].size() == subsizes[cr].size());
    for (auto c : child[v]) {
      self(self, c);
    }
  };
  dfs_compo(dfs_compo, 0);
  DEBUG(compo);
  DEBUG(subsizes);

  // centroid of subtree
  auto query = [&](int v) -> int {
    int total = hld.sizes[v];
    if (total <= 2) {
      return v;
    }
    auto [croot, cid] = compo[v];
    const auto &sz = subsizes[croot];
    int tv = cid, fv = ssize(sz);
    while (fv - tv > 1) {
      int mid = (fv + tv) / 2;
      if (sz[mid] > total / 2) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return compo_node[croot][tv];
  };

  REP(i, Q) {
    int v;
    cin >> v;
    --v;
    cout << query(v) + 1 << '\n';
  }
}
