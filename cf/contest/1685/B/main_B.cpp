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
    return std::cout << (x ? "YES" : "NO") << endc;
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

void exit_() {
  std::string unused;
  assert(not(std::cin >> unused));
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
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

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

template<class T>
std::vector<std::pair<T, int>> runlength(const std::vector<T> &a) {
  std::vector<std::pair<T, int>> res;
  const int n = a.size();
  if (n == 0) return res;
  res.reserve(n);
  res.emplace_back(a[0], 1);
  for (int i = 1; i < n; ++i) {
    if (a[i] == res.back().first) {
      res.back().second++;
    } else {
      res.emplace_back(a[i], 1);
    }
  }
  return res;
}

bool solve() {
  int a = in, b = in, ab = in, ba = in;
  string s = in;
  DUMP(s, a, b, ab, ba);
  const int n = ssize(s);
  vector<int> v(n);
  REP(i, n) {
    v[i] = (s[i] - 'A');
    if (i % 2 == 1) {
      v[i] ^= 1;
    }
  }
  DUMP(v);
  auto rl = runlength(v);
  int offs = 0;
  REP(i, ssize(rl)) {
    rl[i].first ^= offs;
    if (rl[i].second % 2 == 1) {
      offs ^= 1;
    }
  }

  DUMP(rl);
  deque<pair<int, int>> ve, vo;
  for (auto [val, cnt]: rl) {
    if (cnt % 2 == 0) {
      int k = min((val == 0 ? ab : ba), cnt / 2);
      (val == 0 ? ab : ba) -= k;
      cnt -= k * 2;
      if (cnt == 0) continue;
    }
    (cnt % 2 ? vo : ve).push_back({val, cnt});
  }
  sort(ALL(vo), greater{});
  sort(ALL(ve), greater{});

  DUMP(a, b, ab, ba, vo, ve);

  REP(i, 2) {
    deque<pair<int, int>> vo2;
    while (not vo.empty()) {
      auto [val, cnt] = vo.front();
      vo.pop_front();
      int k = min((i == 0 ? ab : ba), cnt / 2);
      (i == 0 ? ab : ba) -= k;
      cnt -= k * 2;
      if (cnt == 0) continue;
      vo2.push_back({val, cnt});
    }
    vo.swap(vo2);
  }
  REP(i, 2) {
    while (not ve.empty()) {
      auto [val, cnt] = ve.front();
      ve.pop_front();
      if (val != i) {
        int k = min((i == 0 ? ab : ba), (cnt - 2) / 2);
        (i == 0 ? ab : ba) -= k;
        (i == 0 ? b : a) -= 1;
        cnt -= k * 2 - 1;
      }
      if (cnt == 0) continue;
      vo.push_back({val, cnt});
    }
  }
  for (auto [cnt, val]: vo) {
    int v2 = cnt / 2;
    int v1 = cnt - v2;

    if (val == 0) {
      a -= v1;
      b -= v2;
    } else {
      b -= v1;
      a -= v2;
    }
  }
  return (a == 0 and b == 0 and ab == 0 and ba == 0);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    DUMP(t);
    print(solve());
  }
  exit_();
}
