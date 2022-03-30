#include <bits/stdc++.h>
#include <algorithm>
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
    return std::cout << (x ? "Alice" : "Bob") << endc;
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

map<tuple<int, int, set<int>>, bool> memo;

bool labo(int n, int slots, set<int> &a) {
  if (a.empty()) return false;
  if (auto it = memo.find(make_tuple(n, slots, a)); it != memo.end()) {
    return it->second;
  }

  int cur = *a.rbegin();
  assert(cur >= n);
  a.erase(cur);

  bool ret = [&] {
    if (slots > 0) {
      bool ok = labo(n, slots - 1, a);
      if (not ok) return true;
    }

    for (int j = cur - 1; j >= n; --j) {
      if (a.count(j)) continue;
      a.insert(j);
      bool ok = labo(n, slots, a);
      a.erase(j);
      if (not ok) return true;
    }

    return false;
  }();
  a.insert(cur);
  memo[make_tuple(n, slots, a)] = ret;
  return ret;
}

bool solve(vector<int> &a) {
  int n = ssize(a);
  vector<int> gaps;
  int slots = n;
  {
    int i = 0;
    while (i < n and a[i] < n) {
      ++i;
      --slots;
    }
    int p = n;
    for (int j = i; j < n; ++j) {
      int gap = (a[j] - p);
      gaps.push_back(gap);
      p = a[j] + 1;
    }
  }

  if (gaps.empty()) {
    return false;
  }
  if (gaps.back() != 0) {
    return true;
  }
  int ret = slots & 1;
  for (int g: gaps) {
    ret ^= (g & 1);
  }
  return ret;

#if 0
  set<int> s(ALL(a));
  int t = n;  // available slots on the left.
  int i = 0;
  for (; i < n; ++i) {
    if (s.count(i)) {
      --t;
      s.erase(i);
    }
  }
  return labo(n, t, s);
#endif
}

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
#if 1
  int n = in;
  vector<int> a = in(n);
  print(solve(a));
#else
  map<tuple<int, vector<int>>, bool> tried;

  REP(iter, 100000) {
    if (iter % 10000 == 0) cerr << "... " << iter << endl;

    int L = 15;
    vector<int> a(L);
    REP(i, L) a[i] = i;
    std::shuffle(ALL(a), PRNG());
    int n = 8;
    while (ssize(a) > n) a.pop_back();
    sort(ALL(a));

    vector<int> gaps;
    int slots = n;
    {
      int i = 0;
      while (i < n and a[i] < n) {
        ++i;
        --slots;
      }
      int p = n;
      for (int j = i; j < n; ++j) {
        int gap = (a[j] - p);
        gaps.push_back(gap);
        p = a[j] + 1;
      }
    }

    bool win = solve(a);

    if (gaps.empty()) {
      assert(not win);
      continue;
    }
    if (gaps.back() != 0) {
      assert(win);
      continue;
    }
    if (all_of(ALL(gaps), [](int x) { return x == 0; })) {
      if (slots % 2 == 0) {
        assert(not win);
      } else {
        assert(win);
      }
      continue;
    }

    tried[make_tuple(slots & 1, gaps)] = win;
  }

  for (auto[k, v]: tried) {
    DUMP(k, v);
  }
#endif
}
