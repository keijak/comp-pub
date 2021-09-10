#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
  for (auto &x: a) is >> x;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct Node {
  bool max;
  bool mex;
  bool l0;
  bool l1;
  bool l2;
  array<Node *, 2> child;
  Node() : max(false), mex(false), l0(false), l1(false), l2(false), child{nullptr, nullptr} {}
};

int mex(int x, int y) {
  if (x != 0 and y != 0) return 0;
  if (x != 1 and y != 1) return 1;
  return 2;
}

auto solve() -> Mint {
  const string s = in;
  const int K = in;
  const int n = s.size();

  auto parse = [&](auto &f, int pos) -> pair<Node *, int> {
    assert(pos < n);
    Node *t = new Node();
    if (s[pos] == 'm') {
      assert(s[pos + 2] == 'x');
      char mc = s[pos + 1];
      if (mc == 'a') {
        t->max = true;
      } else if (mc == 'e') {
        t->mex = true;
      } else if (mc == '?') {
        t->max = t->mex = true;
      } else {
        assert(false);
      }
      assert(s[pos + 3] == '(');
      auto[c1, j1] = f(f, pos + 4);
      assert(s[j1] == ',');
      auto[c2, j2] = f(f, j1 + 1);
      assert(s[j2] == ')');
      t->child[0] = c1;
      t->child[1] = c2;
      return {t, j2 + 1};
    }

    if (s[pos] == '0') {
      t->l0 = true;
    } else if (s[pos] == '1') {
      t->l1 = true;
    } else if (s[pos] == '2') {
      t->l2 = true;
    } else if (s[pos] == '?') {
      t->l0 = t->l1 = t->l2 = true;
    } else {
      assert(false);
    }
    return {t, pos + 1};
  };

  auto[t, pos] = parse(parse, 0);
  assert(pos == n);

  /*
  auto ptree = [&](auto &f, Node *node, int d) -> void {
    REP(i, d) cerr << "  ";
    if (node->max or node->mex) {
      cerr << "(a:" << node->max << ", e:" << node->mex << "\n";
      f(f, node->child[0], d + 1);
      f(f, node->child[1], d + 1);
      REP(i, d) cerr << "  ";
      cerr << ")\n";
    } else {
      cerr << "(0:" << node->l0 << ", 1:" << node->l1 << ", 2:" << node->l2 << ")\n";
    }
  };
  ptree(ptree, t, 0);
*/

  auto dfs = [&](auto &f, Node *node) -> array<Mint, 3> {
    array<Mint, 3> ret = {0, 0, 0};
    if (node->max or node->mex) {
      auto r0 = f(f, node->child[0]);
      auto r1 = f(f, node->child[1]);
      if (node->max) {
        REP(i, 3) REP(j, 3) {
            ret[max(i, j)] += r0[i] * r1[j];
          }
      }
      if (node->mex) {
        REP(i, 3) REP(j, 3) {
            ret[mex(i, j)] += r0[i] * r1[j];
          }
      }
    } else {
      if (node->l0) ret[0]++;
      if (node->l1) ret[1]++;
      if (node->l2) ret[2]++;
    }
    return ret;
  };
  auto res = dfs(dfs, t);
  DUMP(res);

  return res[K];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
