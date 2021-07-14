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

auto solve() {
  INPUT(int, n);
  vector<pair<string, string>> ps(n);// prefix suffix
  map<pair<string, string>, int> node_id_map;
  vector<int> node_id(n, -1);

  map<string, set<int>> pm, sm;
  REP(i, n) {
    string s;
    cin >> s;
    string prefix = s.substr(0, 3);
    string suffix = s.substr(ssize(s) - 3, 3);
    ps[i] = {prefix, suffix};

    if (auto it = node_id_map.find(ps[i]); it != node_id_map.end()) {
      node_id[i] = it->second;
    } else {
      int new_id = ssize(node_id_map);
      node_id[i] = new_id;
      node_id_map[ps[i]] = new_id;
    }
    pm[prefix].insert(node_id[i]);
    sm[suffix].insert(node_id[i]);

    DUMP(s, prefix, suffix, node_id[i]);
  }

  DUMP(pm);
  DUMP(sm);

  const int nn = ssize(node_id_map);
  vector<set<int>> prevs(nn);
  vector<int> deg(nn, 0);
  for (const auto&[key, nid] : node_id_map) {
    const auto&[prefix, suffix] = key;
    prevs[nid] = sm[prefix];
    deg[nid] = ssize(pm[suffix]);
  }
//  REP(v, nn) {
//    DUMP(v, deg[v], prevs[v]);
//  }

  // 0: lose, +1, win, -1: unknown
  vector<int> state(nn, -1);
  queue<int> q;

  REP(v, nn) {
    if (deg[v] == 0) {
      state[v] = 0;
      q.push(v);
    }
  }

  while (not q.empty()) {
    int v = q.front();
    DUMP(v);
    q.pop();
    auto &pnodes = prevs[v];
    auto it = pnodes.begin();
    while (it != pnodes.end()) {
      int u = *it;
      if (state[u] != -1) {
        ++it;
        continue;
      }
      it = pnodes.erase(it);
      deg[u]--;
      if (state[v] == 0) {
        state[u] = 1; // win
        q.push(u);
      } else if (state[v] == 1) {
        if (deg[u] == 0) {
          state[u] = 0;
          q.push(u);
        }
      }
    }
  }
  DUMP(state);

  REP(i, n) {
    int v = node_id[i];
    if (state[v] == 1) {
      print("Aoki");
    } else if (state[v] == 0) {
      print("Takahashi");
    } else {
      print("Draw");
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    solve();
  }
}
