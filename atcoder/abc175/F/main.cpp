#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const i64 INF = 1LL << 50;

struct Node {
  bool terminal;
  i64 cost;
  array<shared_ptr<Node>, 26> child;

  Node() : terminal(false), cost(INF) {}
  explicit Node(i64 c) : terminal(true), cost(c) {}
};

void trie_insert(Node* trie, const string& s, i64 cost) {
  Node* node = trie;
  int n = s.size();
  REP(i, n) {
    int c = s[i] - 'a';
    Node* nx = node->child[c].get();
    if (nx == nullptr) {
      nx = new Node();
      node->child[c].reset(nx);
    }
    node = nx;
  }
  node->terminal = true;
  node->cost = min(node->cost, cost);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  V<string> S(n);
  V<i64> cost(n);
  V<bool> singleton(n);

  shared_ptr<Node> prefix_trie(new Node());
  shared_ptr<Node> suffix_trie(new Node());
  i64 ans = INF;
  REP(i, n) {
    string r;
    cin >> r >> cost[i];
    S[i] = r;
    reverse(r.begin(), r.end());
    if (S[i] == r) {
      singleton[i] = true;
      ans = min(ans, cost[i]);
      continue;
    }
    DEBUG(S[i], r);
    trie_insert(prefix_trie.get(), S[i], cost[i]);
    trie_insert(suffix_trie.get(), r, cost[i]);
  }

  set<pair<string_view, bool>> seen;
  auto rec = [&](auto self, string_view piece, i64 cur_cost, bool rev) -> void {
    if (cur_cost >= ans) return;
    if (piece.empty()) {
      ans = min(ans, cur_cost);
      return;
    }

    pair<string_view, bool> state = {piece, rev};
    if (seen.count(state)) return;
    seen.emplace(piece, rev);
    Node* trie = rev ? suffix_trie.get() : prefix_trie.get();
    if (trie == nullptr) return;
    REP(i, piece.size()) {
      int c = piece[i] - 'a';
      Node* nx = trie->child[c].get();
      if (nx == nullptr) {
        return;
      }
      if (nx->terminal) {
        string_view piece2 = piece.substr(i + 1);
        self(self, piece2, cur_cost + nx->cost, rev);
      }
    }

    auto rec2 = [&](auto self2, Node* node, string prefix) -> void {
      if (node == nullptr) return;
      if (node->terminal) {
        self(self, prefix, cur_cost + node->cost, !rev);
      }
      REP(c, 26) {
        Node* p = node->child[c].get();
        if (p != nullptr) {
          self2(self2, p, prefix + (char)('a' + c));
        }
      }
    };
    string prefix;
    rec2(rec2, trie, prefix);
  };

  REP(i, n) {
    if (singleton[i]) continue;
    rec(rec, S[i], cost[i], true);
  }

  cout << (ans == INF ? -1 : ans) << endl;
}
