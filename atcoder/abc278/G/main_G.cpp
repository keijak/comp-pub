// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

[[noreturn]] void exit_() {
  fflush(stdout), fflush(stderr);
  std::cout.flush(), std::cerr.flush();
  std::_Exit(0);
}

void TLE() {
  double x = 0.0;
  while (true) { x = std::cos(x); }
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<int kMaxSize, int kMaxValue>
struct SparseIntSet {
  int size_;
  int dense_[kMaxSize];
  int sparse_[kMaxValue + 1];

  SparseIntSet() noexcept: size_(0) {}

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

  using iterator = int *;
  using const_iterator = const int *;
  iterator begin() { return dense_; }
  const_iterator begin() const { return dense_; }
  iterator end() { return dense_ + size_; }
  const_iterator end() const { return dense_ + size_; }

  // For silencing warnings on accessing uninitialized memory.
  void safe_init() {
    dense_.fill(0);
    sparse_.fill(0);
  }
};
using IntSet = SparseIntSet<200'005, 200'005>;

int msb_log(unsigned x) {
  assert(x != 0);
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
}
int msb_log(Uint x) {
  assert(x != 0);
  return std::numeric_limits<Uint>::digits - __builtin_clzll(x) - 1;
}
template<typename T, typename U = std::make_unsigned_t<T>>
U msb(T x) {
  if (x == 0) return 0;
  return U(1) << msb_log(U(x));
}

pair<int, int> read_input() {
  int x, y;
  cin >> x >> y;
  return {x, y};
}

void write_move(int x, int y) {
  cout << x << ' ' << y << endl;
}

void mirror_strategy(int N, int L, int R) {
  cout << "First" << endl;
  int e = L;
  if (L != R and (N - L) % 2 != 0) ++e;
  int A = (N - e) / 2;
  cout << (A + 1) << ' ' << (N - 2 * A) << endl;
  int B = N - A + 1;
  while (true) {
    auto [x, y] = read_input();
    if (x == 0 and y == 0) {
      exit_();
    }
    if (x == -1 and y == -1) {
      exit_();
    }
    if (x >= B) {
      int gap = x - B;
      write_move(A - gap - y + 1, y);
    } else {
      int gap = A - (x + y - 1);
      write_move(B + gap, y);
    }
  }
}

void nimber_strategy(int N, int L) {
  vector<int> nimber(N + 1, 0);
  for (int n = 1; n <= N; ++n) {
    set<int> gs;
    for (int i = 0; i <= n + 100; ++i) {
      gs.insert(i);
    }
    for (int x = 0; x + L <= n; ++x) {
      int y = n - (x + L);
      gs.erase(nimber[x] ^ nimber[y]);
    }
    assert (not gs.empty());
    nimber[n] = *gs.begin();
  }
  DUMP(nimber);

  set<pair<int, int>> cards;
  cards.insert(pair{1, N});

  auto erase_interval = [&](int x, bool do_assert) {
    auto it = cards.upper_bound({x, kBig<int>});
    if (it == cards.begin()) {
      TLE();
    }
    --it;
    auto [a, k] = *it;
    if (a <= x and x + L <= a + k) {
      cards.erase(it);
      if (x > a) {
        cards.insert(pair{a, x - a});
      }
      int r = a + k - (x + L);
      if (r > 0) {
        cards.insert(pair{x + L, r});
      }
      return;
    }
    if (do_assert) {
      assert(false);
    } else {
      TLE();
    }
  };

  auto play = [&]() {
    int cur_nimber = 0;
    for (auto [a, k]: cards) {
      cur_nimber ^= nimber[k];
    }
    if (cur_nimber == 0) {
      assert(false);
    }
    uint32_t high_bit = msb(cur_nimber);

    for (auto it = cards.begin(); it != cards.end(); ++it) {
      auto [a, k] = *it;
      if (not(nimber[k] & high_bit)) continue;
      int target = nimber[k] ^ cur_nimber;
      for (int x = a; x + L <= a + k; ++x) {
        int p = x - a;
        int q = (a + k) - (x + L);
        int g = nimber[p] ^ nimber[q];
        if (g == target) {
          erase_interval(x, false);
          write_move(x, L);
          return;
        }
      }
    }
    //assert(false);
    TLE();
  };

  if (nimber[N] == 0) {
    cout << "Second" << endl;
  } else {
    cout << "First" << endl;
    play();
  }

  while (true) {
    auto [x, y] = read_input();
    if (x == 0 and y == 0) {
      exit_();
    }
    if (x == -1 and y == -1) {
      exit_();
    }
    if (y != L) {
      assert(false);
    }
    erase_interval(x, false);
    play();
  }
}

int main() {
  int N, L, R;
  cin >> N >> L >> R;
  if (L != R or (N - L) % 2 == 0) {
    mirror_strategy(N, L, R);
  } else {
    nimber_strategy(N, L);
  }
}
