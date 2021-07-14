#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
using i128 = boost::multiprecision::int128_t;
// using Int = boost::multiprecision::cpp_int;
using Int = long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    --num_roots;
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < n; ++i) {
      if (parent[i] < 0) res.push_back(i);
    }
    return res;
  }
};

vector<int> solve() {
  int n;
  cin >> n;
  vector<Int> a(n);
  REP(i, n) cin >> a[i];

  if (n == 2) {
    vector<int> ans = {1, 2, 1};
    return ans;
  }

  vector<int> ix(n);
  REP(i, n) ix[i] = i;
  sort(ALL(ix), [&](int i, int j) { return a[i] < a[j]; });

  priority_queue<tuple<Int, int, int>> pq;
  for (int i = n - 1; i > 0; --i) {
    int vi = ix[i];
    int vj = ix[i - 1];
    pq.emplace(a[vi] * a[vj], i, i - 1);
  }

  UnionFind uf(n);
  map<int, vector<int>> visited;
  int count = 0;
  while (pq.size() and count < n) {
    auto [prod, i, j] = pq.top();
    pq.pop();
    int vi = ix[i];
    int vj = ix[j];
    if (ssize(visited[vi]) < 2 and ssize(visited[vj]) < 2) {
      if (count == n - 1 or not uf.same(vi, vj)) {
        uf.unite(vi, vj);
        ++count;
        visited[vi].push_back(vj);
        visited[vj].push_back(vi);
        DUMP(count, vi, vj, visited);
      }
    }
    if (count == n) break;
    if (j > 0 and ssize(visited[vi]) < 2) {
      Int ak = a[ix[j - 1]];
      pq.emplace(a[vi] * ak, i, j - 1);
    }
  }
  assert(count == n);

  int cur = 0;
  vector<int> ans;
  ans.push_back(1);
  vector<char> done(n);
  int done_count = 0;
  while (done_count < n) {
    for (auto x : visited[cur]) {
      if (done[x]) continue;
      done[x] = 1;
      done_count += 1;
      ans.push_back(x + 1);
      cur = x;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  print_seq(solve());
}
