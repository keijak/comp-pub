// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<int alphabet_size = 26, int alphabet_base = 'a', int ROOT_ID = 0>
struct Trie {
  struct Node {
    int chr;  // character
    std::vector<std::optional<int>> next;  // child node indices
    std::vector<int> entries;  // entry ids stored at the terminal node
    int prefix_count;           // how many entries are stored below this node.
    Node(int c) : chr(c), next(alphabet_size), prefix_count(0) {}
  };
  std::vector<Node> nodes;

  Trie() : nodes(1, Node{ROOT_ID}) {}

  pair<const Node *, int> insert(std::string_view entry, int entry_id) {
    int node_id = ROOT_ID;
    int pcount = 0;
    for (char ch: entry) {
      pcount += ssize(nodes[node_id].entries);
      ++(nodes[node_id].prefix_count);
      int c = int(ch) - alphabet_base;
      auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) {
        next_id = (int) nodes.size();
        nodes.emplace_back(c);
      }
      node_id = next_id.value();
    }
    Node *node = &nodes[node_id];
    ++(node->prefix_count);
    node->entries.push_back(entry_id);
    return {node, pcount};
  }
  pair<const Node *, int> insert(std::string_view entry) {
    return insert(entry, nodes[ROOT_ID].prefix_count);
  }

  const Node *search(std::string_view entry) const {
    int node_id = ROOT_ID;
    for (char ch: entry) {
      int c = int(ch) - alphabet_base;
      const auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) return nullptr;
      node_id = next_id.value();
    }
    return &nodes[node_id];
  }

  bool contains(std::string_view entry) const {
    auto res = search(entry);
    if (res == nullptr) return false;
    return not res->entries.empty();
  }

  bool contains_prefix(std::string_view entry) const {
    return search(entry) != nullptr;
  }
};

// mod: prime
template<class T = Mint>
struct BinomialCoeff {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  explicit BinomialCoeff(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (binomial coefficients)
  T operator()(Int n, Int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

auto solve() {
  int n = in;
  vector<string> S(n);
  REP(i, n) {
    cin >> S[i];
  }
  vector<int> idx(n);
  REP(i, n) idx[i] = i;
  sort(ALL(idx), [&](int i, int j) {
    return S[i].size() < S[j].size();
  });
  const Mint kHalf = Mint(1) / 2;
  vector<int> pcounts(n);
  Trie<> trie;
  REP(i, n) {
    string_view s = S[idx[i]];
    auto [node, pcount] = trie.insert(s, i);
    DUMP(i, S[idx[i]], pcount, node->entries);
    pcounts[idx[i]] = pcount;
  }
  vector<Mint> ans(n);
  REP(i, n) {
    int ri = idx[i];
    const int pcount = pcounts[ri];
    string_view s = S[ri];
    const auto *node = trie.search(s);
    assert(node != nullptr);
    const int qcount = node->prefix_count;
    Mint res = 1 + pcount + (n - pcount - qcount) * kHalf;
    ans[ri] = res;
  }
  out_seq(ans, "\n");
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
