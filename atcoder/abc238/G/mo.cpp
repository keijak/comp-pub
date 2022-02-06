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

const int L = int(1e6) + 100;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1) {
    // O(n)
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (const auto &p: primes) {
        if (i * p > n or p > spf[i]) break;
        spf[i * p] = p;
      }
    }
  }

  inline bool is_prime(int n) const { return spf[n] == n; }

  auto factorize(int n) const {
    assert(0 < n and n < int(spf.size()));
    std::vector<std::pair<Int, int>> res;
    while (n > 1) {
      const int p = spf[n];
      int count = 0;
      do {
        n /= p;
        ++count;
      } while (n % p == 0);
      res.emplace_back(p, count);
    }
    return res;
  }
} sieve(L);

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  int n;
  vector<int> answer;
  vector<vector<pair<Int, int>>> facs;
  vector<int> counter;
  int nonzero_count = 0;

  Task(int N, int Q) : n(N), answer(Q), facs(N), counter(L, 0) {}

  void add_left(int l) { add_one(l); }
  void add_right(int r) { add_one(r); }
  void erase_left(int l) { erase_one(l); }
  void erase_right(int r) { erase_one(r); }

  // Adds one item at position i.
  void add_one(int i) {
    for (const auto&[p, k]: facs[i]) {
      int pc = counter[p];
      counter[p] += k;
      counter[p] %= 3;
      int cc = counter[p];
      if (pc == 0 and cc != 0) {
        ++nonzero_count;
      }
      if (pc != 0 and cc == 0) {
        --nonzero_count;
      }
    }
  }

  // Erases one item at position i.
  void erase_one(int i) {
    for (const auto&[p, k]: facs[i]) {
      int pc = counter[p];
      counter[p] -= k;
      counter[p] %= 3;
      int cc = counter[p];
      if (pc == 0 and cc != 0) {
        ++nonzero_count;
      }
      if (pc != 0 and cc == 0) {
        --nonzero_count;
      }
    }
  }

  // Processes one query on the current state.
  void process(const Query &q) {
    answer[q.idx] = nonzero_count == 0;
  }
};

struct Mo {
  int n;
  vector<Query> queries;

  explicit Mo(int n) : n(n) {}

  void add_query(Query query) { queries.push_back(query); }

  void solve(Task &task) const {
    int q = (int) queries.size();
    const int B = 450; //n / min<int>(n, sqrt(q));
    vector<int> ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / B;
      int bblock = queries[b].l / B;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries[a].r > queries[b].r
                          : queries[a].r < queries[b].r;
    });
    int l = 0, r = 0;
    for (auto idx: ord) {
      while (l > queries[idx].l) task.add_left(--l);
      while (r < queries[idx].r) task.add_right(r++);
      while (l < queries[idx].l) task.erase_left(l++);
      while (r > queries[idx].r) task.erase_right(--r);
      task.process(queries[idx]);
    }
  }
};

auto solve() {
  int n = in, Q = in;
  vector<Int> a = in(n);
  Task task(n, Q);
  REP(i, n) {
    task.facs[i] = sieve.factorize(a[i]);
  }
  Mo mo(n);
  REP(i, Q) {
    int l = in, r = in;
    --l;
    mo.add_query(Query{i, l, r});
  }
  mo.solve(task);
  REP(i, Q) {
    print((bool) task.answer[i]);
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
