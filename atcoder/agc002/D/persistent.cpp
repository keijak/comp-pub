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

template <typename T, int K = 20>
class PersistentArray {
 public:
  struct Node;
  // No memory release by default.
  using NodePtr = Node *;  // std::shared_ptr<Node>;

  struct Node {
    std::optional<T> val;
    std::unique_ptr<std::array<NodePtr, K>> children;
  };

  explicit PersistentArray(NodePtr root) : root_{std::move(root)} {}
  PersistentArray() : root_{} {}
  PersistentArray(const PersistentArray &) = default;
  PersistentArray(PersistentArray &&) = default;
  PersistentArray &operator=(const PersistentArray &) = default;
  PersistentArray &operator=(PersistentArray &&) = default;

  std::optional<T> operator[](int idx) const { return do_get(idx, root_); }

  PersistentArray<T, K> set(int idx, T val) const {
    return PersistentArray<T, K>(do_set(idx, val, root_));
  }

 private:
  static std::optional<T> do_get(int idx, const NodePtr &node) {
    if (node == nullptr) return std::nullopt;
    if (idx == 0) return node->val;
    if (node->children == nullptr) return std::nullopt;
    const NodePtr &child = (*node->children)[idx % K];
    if (child == nullptr) return std::nullopt;
    return do_get(idx / K, child);
  }

  static NodePtr do_set(int idx, T val, const NodePtr &node) {
    NodePtr res(new Node());
    if (node != nullptr) {
      res->val = node->val;
      if (node->children != nullptr) {
        res->children.reset(new std::array<NodePtr, K>(*node->children));
      }
    }
    if (idx == 0) {
      res->val = std::move(val);
    } else {
      if (res->children == nullptr) {
        res->children.reset(new std::array<NodePtr, K>());
      }
      (*res->children)[idx % K] =
          do_set(idx / K, std::move(val), (*res->children)[idx % K]);
    }
    return res;
  }

 private:
  NodePtr root_;
};

class PersistentUnionFind {
 public:
  explicit PersistentUnionFind(PersistentArray<int> par)
      : parent_{std::move(par)} {}
  PersistentUnionFind() {}
  PersistentUnionFind(const PersistentUnionFind &) = default;
  PersistentUnionFind(PersistentUnionFind &&) = default;
  PersistentUnionFind &operator=(const PersistentUnionFind &) = default;
  PersistentUnionFind &operator=(PersistentUnionFind &&) = default;

  PersistentUnionFind unite(int x, int y) const {
    x = find(x);
    y = find(y);
    if (x == y) return *this;
    const int x_size = -parent_[x].value_or(-1);
    const int y_size = -parent_[y].value_or(-1);
    // Ensure |x| >= |y|.
    if (x_size < y_size) std::swap(x, y);
    return PersistentUnionFind(parent_.set(x, -(x_size + y_size)).set(y, x));
  }

  int find(int x) const {
    const std::optional<int> &par = parent_[x];
    if (not par or *par < 0) return x;
    return find(*par);
  }

  bool same(int x, int y) const { return find(x) == find(y); }

  int size(int x) const {
    int root = find(x);
    return -(parent_[root].value_or(-1));
  }

 private:
  PersistentArray<int> parent_;
};

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<PersistentUnionFind> ufs(m + 1);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    ufs[i + 1] = ufs[i].unite(a, b);
  }

  auto score_separate = [&](int x, int y, int z) -> int {
    int fv = 0, tv = m;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      int sz1 = ufs[mid].size(x);
      int sz2 = ufs[mid].size(y);
      if (sz1 + sz2 >= z) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  };

  auto score_united = [&](int x, int z) -> int {
    int fv = 0, tv = m;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      int sz = ufs[mid].size(x);
      if (sz >= z) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  };

  int q;
  cin >> q;
  REP(i, q) {
    int x, y, z;
    cin >> x >> y >> z;
    --x, --y;
    int j = score_separate(x, y, z);
    if (ufs[j].same(x, y)) {
      j = score_united(x, z);
    }
    cout << j << "\n";
  }
}
