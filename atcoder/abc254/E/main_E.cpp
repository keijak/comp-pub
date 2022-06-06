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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    assert(std::cin >> x);
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) assert(std::cin >> x);
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));  // No input is left behind.
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

inline void init_test_case(int t, int T) {
#ifdef MY_DEBUG
  if (T > 1) {
    std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
              << std::endl;
  }
#endif
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;
template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

// s |= t;
// Set Union.
// The first argument will be updated to the result.
// cf. https://qiita.com/i_saint/items/a8bdce5146bb38e69f72
template<typename S = std::set<int>>
S &operator|=(S &s, const S &t) {
  auto pos = s.begin();
  for (auto it = t.begin(); it != t.end(); ++it, ++pos) {
    pos = s.emplace_hint(pos, *it);
  }
  return s;
}
template<typename S = std::set<int>>
S &operator|=(S &s, S &&t) {
  if (s.size() < t.size()) s.swap(t);  // small-to-large merging
  for (auto pos = s.begin(); !t.empty(); ++pos) {
    pos = s.insert(pos, t.extract(t.begin()));
  }
  return s;
}

struct SparseSet {
  static constexpr int kMaxSize = int(2e5) + 10;
  static constexpr int kMaxValue = int(2e5) + 10;

  int size_ = 0;
  std::array<int, kMaxSize> dense_;
  std::array<int, kMaxValue> sparse_;

  // O(1)
  bool contains(int x) const {
    // May access uninitialized memory, but safe.
    const size_t i = sparse_[x];
    return i < (size_t) size_ and dense_[i] == x;
  }

  // O(1)
  void insert(int x) {
    if (contains(x)) return;
    dense_[size_] = x;
    sparse_[x] = size_++;
  }

  // O(1)
  void erase(int x) {
    const int i = sparse_[x];
    if (i < 0 or i >= size_ or dense_[i] != x) return;
    const auto y = dense_[--size_];
    dense_[i] = y;
    sparse_[y] = i;
  }

  // O(1)
  inline void clear() { size_ = 0; }
  inline int size() const { return size_; }
  inline bool empty() const { return size_ == 0; }

  // O(size)
  template<class F>
  void for_each(F func) {
    for (int i = 0; i < size_; ++i) {
      func(dense_[i]);
    }
  }

  // For silencing warnings on accessing uninitialized memory.
  void safe_init() {
    dense_.fill(0);
    sparse_.fill(0);
  }
};

auto solve() {
  int n = in, m = in;
  vector<vector<int>> g(n);
  REP(i, m) {
    int a = in, b = in;
    a--;
    b--;
    g[a].push_back(b);
    g[b].push_back(a);
  }

  SparseSet visited;

  auto f = Rec([&](auto &rec, int x, int K) -> Int {
    queue<pair<int, int>> q;
    q.emplace(0, x);
    visited.clear();
    visited.insert(x);
    Int res = 0;
    while (not q.empty()) {
      auto [d, v] = q.front();
      q.pop();
      res += v + 1;
      if (d == K) continue;
      for (auto u: g[v]) {
        if (visited.contains(u)) continue;
        visited.insert(u);
        q.emplace(d + 1, u);
      }
    }
    return res;
  });
  int Q = in;
  REP(qi, Q) {
    int x = in, K = in;
    --x;
    visited.clear();
    auto s = f(x, K);
    print(s);
  }
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    init_test_case(t, T);
    (solve());
  }
  exit_();
}
