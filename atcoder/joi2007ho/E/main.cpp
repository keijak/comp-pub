#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
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

struct Node {
  int id;
  int p;
  int q;
  Node *r = nullptr;
  Node *b = nullptr;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;  // <= 100
  cin >> N;
  V<int> indeg(N);
  V<unique_ptr<Node>> nodes(N);
  REP(i, N) { nodes[i].reset(new Node{i, 0, 0, nullptr, nullptr}); }
  REP(i, N) {
    int p, q, r, b;
    cin >> p >> q >> r >> b;
    int g = gcd(p, q);
    nodes[i]->p = p / g;
    nodes[i]->q = q / g;
    if (r != 0) {
      nodes[i]->r = nodes[r - 1].get();
      indeg[r - 1]++;
    }
    if (b != 0) {
      nodes[i]->b = nodes[b - 1].get();
      indeg[b - 1]++;
    }
  }

  int root = -1;
  REP(i, N) {
    if (indeg[i] == 0) {
      assert(root == -1);
      root = i;
    } else {
      assert(indeg[i] == 1);
    }
  }
  DEBUG(root);

  auto dfs = [&](auto self, int v) -> i64 {
    i64 w1 = 1, w2 = 1;
    if (nodes[v]->r) {
      w1 = self(self, nodes[v]->r->id);
    }
    if (nodes[v]->b) {
      w2 = self(self, nodes[v]->b->id);
    }
    i64 t1 = nodes[v]->p * w1;
    i64 t2 = nodes[v]->q * w2;
    i64 g = gcd(t1, t2);
    i64 lcm = t1 / g * t2;
    return lcm / nodes[v]->p + lcm / nodes[v]->q;
  };
  cout << dfs(dfs, root) << endl;
}
