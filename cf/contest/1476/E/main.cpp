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

static const int ROOT_ID = 0;

template <int alphabet_size = 27, int alphabet_base = 'a'>
struct Trie {
  struct Node {
    int c;                                 // character
    std::vector<std::optional<int>> next;  // child node indices
    std::vector<int> entries;  // entry ids stored at the terminal node
    int entry_count;           // how many entries are stored below this node.
    Node(int c_) : c(c_), next(alphabet_size), entry_count(0) {}
  };
  std::vector<Node> nodes;

  Trie() : nodes(1, Node{ROOT_ID}) {}

  void insert(std::string_view entry, int entry_id) {
    int node_id = ROOT_ID;
    for (char ch : entry) {
      ++(nodes[node_id].entry_count);
      int c = (ch == '_') ? 26 : int(ch) - alphabet_base;
      auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) {
        next_id = (int)nodes.size();
        nodes.emplace_back(c);
      }
      node_id = next_id.value();
    }
    ++(nodes[node_id].entry_count);
    nodes[node_id].entries.push_back(entry_id);
  }
  void insert(std::string_view entry) {
    insert(entry, nodes[ROOT_ID].entry_count);
  }

  void search(const string &entry, int i, int node_id, set<int> &matches) {
    if (i == ssize(entry)) {
      const auto &e = nodes[node_id].entries;
      matches.insert(e.begin(), e.end());
      return;
    }
    {
      char ch = entry[i];
      int c = int(ch) - alphabet_base;
      const auto &next_id = nodes[node_id].next[c];
      if (next_id.has_value()) {
        int nid = next_id.value();
        search(entry, i + 1, nid, matches);
      }
    }
    {
      int c = 26;
      const auto &next_id = nodes[node_id].next[c];
      if (next_id.has_value()) {
        int nid = next_id.value();
        search(entry, i + 1, nid, matches);
      }
    }
  }
};

// Returns the topogically sorted list of nodes.
// Nodes with zero indegree come first.
// Returns nullopt if the input graph is not a DAG.
optional<vector<int>> topological_sort(const vector<set<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u : g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) q.emplace_back(v);
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u : g[v]) {
      if (--indeg[u] == 0) q.push_back(u);
    }
  }
  if (int(res.size()) != n) return nullopt;  // not a DAG
  return res;
}

optional<vector<int>> solve() {
  int n, m, k;
  cin >> n >> m >> k;
  Trie<> trie;
  REP(i, n) {
    string pat;
    cin >> pat;
    trie.insert(pat, i);
  }
  bool no_match_found = false;
  vector<set<int>> g(n);
  REP(i, m) {
    string s;
    int mt;
    cin >> s >> mt;
    --mt;
    DEBUG(s, mt);
    set<int> matches;
    trie.search(s, 0, ROOT_ID, matches);
    if (not matches.count(mt)) {
      no_match_found = true;
    } else {
      for (auto j : matches) {
        if (j == mt) continue;
        g[mt].insert(j);
      }
    }
  }
  if (no_match_found) return nullopt;
  auto res = topological_sort(g);
  if (!res) return nullopt;
  for (auto &x : res.value()) ++x;  // to 1-indexed
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  if (!ans) {
    cout << "NO\n";
  } else {
    cout << "YES\n";
    print_seq(ans.value());
  }
}
