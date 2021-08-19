#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
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
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

struct Edge {
  int to;
  int eid;
};

auto solve() -> optional<vector<int>> {
  INPUT(int, n, m);
  map<int, int> bomb;
  vector<int> as;
  REP(i, n) {
    INPUT(int, a, b);
    auto it = bomb.find(a);
    if (it == bomb.end()) {
      bomb[a] = b;
      as.push_back(a);
    } else {
      if (b != it->second) return nullopt;
    }
  }
  Compressed<int> ca(as);
  int p = ca.size();
  vector<int> on(p), diff(p + 1);
  {
    int i = 0;
    for (auto[a, b] : bomb) {
      on[i] = b;
      diff[i] = (on[i] != (i == 0 ? 0 : on[i - 1]));
      i++;
    }
    assert(i == p);
    diff[p] = on[p - 1] != 0;
  }
  DUMP(p, diff);

  vector<vector<Edge>> g(p + 1);
  REP(i, m) {
    INPUT(int, l, r);
    int li = ca.index(l);
    int ri = ca.index(r + 1);
    g[li].push_back(Edge{ri, i + 1});
    g[ri].push_back(Edge{li, i + 1});
  }

  vector<int> done(p + 1, false);
  vector<int> ans;

  auto dfs = [&](auto &dfs, int v) -> bool {
    bool active = diff[v];
    for (const auto &e : g[v]) {
      if (done[e.to]) continue;
      done[e.to] = true;
      if (dfs(dfs, e.to)) {
        ans.push_back(e.eid);
        active = not active;
      }
    }
    return active;
  };
  REP(v, p + 1) {
    if (done[v]) continue;
    done[v] = true;
    bool active = dfs(dfs, v);
    if (active) return nullopt;
  }
  sort(ALL(ans));
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print(-1);
    } else {
      print(ans.value().size());
      print_seq(ans.value());
    }
  }
}
