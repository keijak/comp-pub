#include <bits/stdc++.h>

#include <atcoder/fenwicktree>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

template <typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

using namespace std;

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  int n;
  vector<i64> answer;
  const vector<int> &a;
  const Compressed<int> &ca;
  atcoder::fenwick_tree<i64> bit;
  i64 inv;

  explicit Task(int n, int num_queries, const vector<int> &a_,
                const Compressed<int> &ca_)
      : n(n), answer(num_queries), a(a_), ca(ca_), bit(ca_.size()), inv(0) {}

  void add_left(int l) {
    int val = ca.index(a[l]);
    inv += bit.sum(0, val);
    bit.add(val, 1);
  }
  void add_right(int r) {
    assert(r < n);
    int val = ca.index(a[r]);
    inv += bit.sum(val + 1, ca.size());
    bit.add(val, 1);
  }
  void erase_left(int l) {
    int val = ca.index(a[l]);
    inv -= bit.sum(0, val);
    bit.add(val, -1);
  }
  void erase_right(int r) {
    int val = ca.index(a[r]);
    inv -= bit.sum(val + 1, ca.size());
    bit.add(val, -1);
  }

  // Processes one query on the current state.
  void process(const Query &q) { answer[q.idx] = inv; }
};

struct Mo {
  int n;
  vector<Query> queries;

  explicit Mo(int n) : n(n) {}

  void add_query(Query query) { queries.push_back(std::move(query)); }

  void solve(Task &task) const {
    int q = queries.size();
    int bs = n / min<int>(n, sqrt(q));
    vector<int> ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / bs;
      int bblock = queries[b].l / bs;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries[a].r > queries[b].r
                          : queries[a].r < queries[b].r;
    });
    int l = 0, r = 0;
    for (auto idx : ord) {
      while (l > queries[idx].l) task.add_left(--l);
      while (r < queries[idx].r) task.add_right(r++);
      while (l < queries[idx].l) task.erase_left(l++);
      while (r > queries[idx].r) task.erase_right(--r);
      task.process(queries[idx]);
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  cin >> A;
  Compressed<int> ca(A);
  Task task(N, Q, A, ca);
  Mo mo(N);
  REP(q, Q) {
    int l, r;
    cin >> l >> r;
    mo.add_query(Query{q, l, r});
  }
  mo.solve(task);
  REP(q, Q) { cout << task.answer[q] << "\n"; }
}
