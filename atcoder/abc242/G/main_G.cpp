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
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
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
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
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

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  int n;
  vector<int> a;
  vector<int> cc;
  vector<Int> answer;
  Int num_pairs;

  Task(int N, int Q) : n(N), cc(N + 5), answer(Q), num_pairs(0) {}

  void add_left(int l) { add_one(l); }
  void add_right(int r) { add_one(r); }
  void erase_left(int l) { erase_one(l); }
  void erase_right(int r) { erase_one(r); }

  // Adds one item at position i.
  void add_one(int i) {
    num_pairs -= cc[a[i]] / 2;
    ++cc[a[i]];
    num_pairs += cc[a[i]] / 2;
  }

  // Erases one item at position i.
  void erase_one(int i) {
    num_pairs -= cc[a[i]] / 2;
    --cc[a[i]];
    num_pairs += cc[a[i]] / 2;
  }

  // Processes one query on the current state.
  void process(const Query &q) {
    answer[q.idx] = num_pairs;
  }
};

struct Mo {
  vector<Query> queries_;
  vector<int> ord_;
  bool build_done_ = false;

  void add_query(Query query) {
    queries_.push_back(std::move(query));
  }

  void build() {
    static constexpr int B = 123;
    const int Q = (int) queries_.size();
    ord_.resize(Q);
    iota(begin(ord_), end(ord_), 0);
    sort(begin(ord_), end(ord_), [&](int a, int b) {
      int ablock = queries_[a].l / B;
      int bblock = queries_[b].l / B;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries_[a].r > queries_[b].r
                          : queries_[a].r < queries_[b].r;
    });
    build_done_ = true;
  }

  void solve(Task &task) {
    if (not build_done_) build();
    int l = 0, r = 0;
    for (auto idx: ord_) {
      while (l > queries_[idx].l) task.add_left(--l);
      while (r < queries_[idx].r) task.add_right(r++);
      while (l < queries_[idx].l) task.erase_left(l++);
      while (r > queries_[idx].r) task.erase_right(--r);
      task.process(queries_[idx]);
    }
  }
};

auto solve() {
  int n = in;
  vector<int> a = in(n);
  int Q = in;
  Task task(n, Q);
  task.a = a;

  Mo mo;
  REP(i, Q) {
    int l = in, r = in;
    mo.add_query(Query{i, l - 1, r});
  }
  mo.solve(task);
  REP(i, Q) {
    print(task.answer[i]);
  }
  return Void();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
