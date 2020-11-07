#include <bits/stdc++.h>

#include <atcoder/modint>
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
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
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
using Mint = atcoder::modint1000000007;

template <int alphabet_size = 26, int alphabet_base = 'a'>
struct Trie {
  struct Node {
    int c;                       // character
    vector<optional<int>> next;  // child node indices
    vector<int> entries;         // entry ids stored at the terminal node
    int entry_count;             // how many entries are stored below this node.
    Node(int c_) : c(c_), next(alphabet_size), entry_count(0) {}
  };
  int root;
  vector<Node> nodes;

  Trie() : root(0), nodes(1, Node{root}) {}

  void insert(string_view entry, int entry_id) {
    int node_id = root;
    for (char ch : entry) {
      ++(nodes[node_id].entry_count);
      int c = int(ch) - alphabet_base;
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
  void insert(string_view entry) { insert(entry, nodes[root].entry_count); }

  optional<const Node *> search(string_view entry) const {
    int node_id = root;
    for (char ch : entry) {
      int c = int(ch) - alphabet_base;
      const auto &next_id = nodes[node_id].next[c];
      if (not next_id.has_value()) return nullopt;
      node_id = next_id.value();
    }
    return &nodes[node_id];
  }

  bool contains(string_view entry) const {
    auto res = search(entry);
    if (not res.has_value()) return false;
    return not res.value()->entries.empty();
  }

  bool contains_prefix(string_view entry) const {
    return search(entry).has_value();
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  string spell;
  cin >> spell;

  Trie trie;
  REP(i, N) {
    string t;
    cin >> t;
    trie.insert(t);
  }
  const int M = spell.size();
  string_view s{spell};
  auto dp = vector(M + 1, Mint(0));
  dp[0] = 1;
  for (int i = 1; i <= M; ++i) {
    for (int j = 0; j < i; ++j) {
      if (dp[j].val() == 0) continue;
      string_view ss = s.substr(j, i - j);
      if (not trie.contains(ss)) continue;
      dp[i] += dp[j];
    }
  }
  cout << dp[M].val() << endl;
}
