// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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

template<int ROOT_ID = 0>
struct AhoCorasick {
  struct Node {
    char ch;                                // character
    std::map<char, int> next;
    std::vector<int> entries;  // entry ids stored at the terminal node
    int prefix_count;          // how many entries are stored below this node.
    int suffix_count;
    int failure_id;

    explicit Node(char c) : ch(c), prefix_count(0), suffix_count(0), failure_id(ROOT_ID) {}
  };
  std::vector<Node> nodes;

  AhoCorasick() : nodes(1, Node{ROOT_ID}) {}

  const Node *insert(std::string_view entry, int entry_id) {
    int node_id = ROOT_ID;
    for (char ch: entry) {
      ++(nodes[node_id].prefix_count);
      auto it = nodes[node_id].next.find(ch);
      if (it == nodes[node_id].next.end()) {
        int next_id = (int) nodes.size();
        nodes[node_id].next[ch] = next_id;
        nodes.emplace_back(ch);
        node_id = next_id;
      } else {
        node_id = it->second;
      }
    }
    Node *node = &nodes[node_id];
    node->prefix_count += 1;
    node->suffix_count += 1;
    node->entries.push_back(entry_id);
    return node;
  }
  const Node *insert(std::string_view entry) {
    return insert(entry, nodes[ROOT_ID].prefix_count);
  }

  void build_failure_links() {
    std::queue<pair<int, int>> q;
    for (auto [ch, next_id]: nodes[ROOT_ID].next) {
      q.emplace(next_id, ROOT_ID);
    }
    while (not q.empty()) {
      int node_id, parent_id;
      std::tie(node_id, parent_id) = q.front();
      q.pop();
      if (parent_id != ROOT_ID) {
        const char ch = nodes[node_id].ch;
        parent_id = nodes[parent_id].failure_id;
        while (true) {
          auto it = nodes[parent_id].next.find(ch);
          if (it != nodes[parent_id].next.end()) {
            nodes[node_id].failure_id = it->second;
            nodes[node_id].suffix_count += nodes[it->second].suffix_count;
            break;
          }
          if (parent_id == ROOT_ID) break;
          parent_id = nodes[parent_id].failure_id;
        }
      }
      for (const auto [ch, next_id]: nodes[node_id].next) {
        q.emplace(next_id, node_id);
      }
    }
  }

  const Node *search(std::string_view entry) const {
    int node_id = ROOT_ID;
    for (char ch: entry) {
      auto it = nodes[node_id].next.find(ch);
      if (it == nodes[node_id].next.end()) return nullptr;
      node_id = it->second;
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

auto solve() {
  string S = in;
  int n = in;
  vector<string> T = in(n);
  AhoCorasick<> ac;
  REP(i, n) {
    ac.insert(T[i]);
  }
  ac.build_failure_links();

  int node_id = 0;  // root
  Int ans = 0;
  for (char ch: S) {
    for (;;) {
      auto &node = ac.nodes[node_id];
      auto it = node.next.find(ch);
      if (it != node.next.end()) {
        node_id = it->second;
        break;
      }
      if (node_id == 0) {
        break;
      }
      node_id = node.failure_id;
    }
    auto &node = ac.nodes[node_id];
    if (node.suffix_count > 0) {
      ++ans;
      node_id = 0;
    }
  }
  out(ans);
}

int main() {
  init_();
  solve();
  exit_();
}
