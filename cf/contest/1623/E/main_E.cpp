#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
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
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct Node {
  int id;
  char ch;
  Node *l;
  Node *r;
  Node *par;
  int depth;
  int dup;
};

auto solve() {
  int n = in, K = in;
  string C = in;
  assert(ssize(C) == n);
  vector<Node> nodes(n);
  REP(i, n) {
    nodes[i] = Node{i, C[i], nullptr, nullptr, nullptr, 0, 0};
  }
  REP(i, n) {
    int l = in, r = in;
    --l, --r;
    if (l != -1) {
      nodes[i].l = &nodes[l];
      nodes[l].par = &nodes[i];
    }
    if (r != -1) {
      nodes[i].r = &nodes[r];
      nodes[r].par = &nodes[i];
    }
  }

  auto set_depth = [&](auto &f, Node *node, int d) -> void {
    node->depth = d;
    if (node->l) f(f, node->l, d + 1);
    if (node->r) f(f, node->r, d + 1);
  };
  set_depth(set_depth, &nodes[0], 0);

  vector<array<char, 2>> sub_fc(n);
  auto fc = [&](auto &fc, Node *node, array<char, 2> pfc) -> array<char, 2> {
    array<char, 2> ret = pfc;
    if (node->r) {
      ret = fc(fc, node->r, pfc);
    }
    sub_fc[node->id] = ret;

    if (ret[0] != node->ch) {
      ret[1] = ret[0];
      ret[0] = node->ch;
    }
    if (node->l) {
      ret = fc(fc, node->l, ret);
    }
    return ret;
  };
  fc(fc, &nodes[0], {'$', '$'});

  auto f = [&](auto &f, Node *node, bool dup_allowed, int dup_depth) -> void {
    if (node->l) {
      f(f, node->l, dup_allowed, dup_depth);
    }
    if (dup_allowed and not node->dup and K > 0 and node->depth - dup_depth <= K) {
      const auto &cs = sub_fc[node->id];
      if (cs[0] > node->ch or (cs[0] == node->ch and cs[1] > node->ch)) {
        for (Node *p = node; p != nullptr and p->depth > dup_depth; p = p->par) {
          p->dup = true;
          --K;
        }
        assert(K >= 0);
      }
    }
    if (node->r) {
      if (node->dup) {
        dup_depth = node->depth;
      } else {
        dup_allowed = false;
      }
      f(f, node->r, dup_allowed, dup_depth);
    }
  };
  f(f, &nodes[0], true, -1);

  auto outp = [&](auto &outp, Node *node) -> void {
    if (node->l) {
      outp(outp, node->l);
    }
    cout << node->ch;
    if (node->dup) cout << node->ch;
    if (node->r) {
      outp(outp, node->r);
    }
  };
  outp(outp, &nodes[0]);
  cout << "\n";
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
