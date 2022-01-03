#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
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

int qcnt;

int query(int i) {
  ++qcnt;
  ++i;
  cout << "? " << i << endl;
  int r;
  cin >> r;
  --r;
  return r;
}

void answer(const vector<int> &p) {
  cout << "!";
  for (auto e: p) {
    cout << ' ' << (e + 1);
  }
  cout << endl;
}

void solve() {
  int n;
  cin >> n;
  qcnt = 0;
  int pos = 0;
  vector<int> ans(n, -1);
  while (pos < n) {
    if (ans[pos] != -1) {
      ++pos;
      continue;
    }
    assert(qcnt < n * 2);
    int x0 = query(pos);
    vector<int> xs = {x0};
    int j = -1;
    if (x0 == pos) j = 0;
    while (true) {
      int x = query(pos);
      if (x == x0) break;
      if (j == -1 and x == pos) {
        j = ssize(xs);
      }
      xs.push_back(x);
    }
    //DUMP(xs);
    std::rotate(xs.begin(), xs.begin() + j, xs.end());
    //DUMP(xs);
    assert(xs[0] == pos);
    REP(i, xs.size()) {
      int z = (i == ssize(xs) - 1) ? pos : xs[i + 1];
      ans[xs[i]] = z;
    }
    //DUMP(ans);
  }
  answer(ans);
}

int main() {
  std::ios::sync_with_stdio(false);
  int T;
  cin >> T;
  REP(t, T) solve();
}
