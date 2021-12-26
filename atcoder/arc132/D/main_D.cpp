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

void flip(string &s) {
  REP(i, ssize(s)) {
    int c = s[i] - '0';
    c = 1 - c;
    s[i] = c + '0';
  }
}

Int beauty(const string &s) {
  int n = s.size();
  Int ret = 0;
  REP(i, 1, n) {
    if (s[i] == s[i - 1]) {
      ++ret;
    }
  }
  return ret;
}

auto solve() {
  int n = in, m = in;
  string S = in;
  string T = in;
  Int ans = 0;
//  chmax(ans, beauty(S));
//  chmax(ans, beauty(T));
  REP(ite, 1) {
    if (n < m) {
      swap(n, m);
      flip(S);
      flip(T);
    }
    vector<int> si, ti;
    REP(i, n + m) {
      if (S[i] == '1') {
        si.push_back(i);
      }
      if (T[i] == '1') {
        ti.push_back(i);
      }
    }
    DUMP(si);
    DUMP(ti);
    assert(ssize(si) == ssize(ti));
    const int Z = ssize(si);

    vector<unordered_map<int, Int>> memo(Z);

    auto f = [&](auto &f, int i, int px) -> Int {
      if (i == Z) {
        return max<Int>((n + m - 1) - px - 1, 0);
      }
      if (auto it = memo[i].find(px); it != memo[i].end()) {
        return it->second;
      }
      int sx = si[i];
      int tx = ti[i];
      Int res = 0;
      //string branch;
      if (sx == tx) {
        Int r = f(f, i + 1, sx);
        if (px + 3 <= sx) r += sx - px - 2;
        if (i > 0 and px + 1 == sx) ++r;
        chmax(res, r);
        //branch = "eq";
      } else if (sx < tx) {
        Int r = f(f, i + 1, tx);
        if (px + 3 <= tx) r += tx - px - 2;
        if (i > 0 and px + 1 == tx) ++r;
        chmax(res, r);
        //branch = "less";
        if (px + 1 >= sx) {
          assert(px + 1 <= tx);
          r = f(f, i + 1, px + 1);
          if (i > 0) ++r;
          if (chmax(res, r)) {
            //  branch += '+';
          }
        }
      } else {
        Int r = f(f, i + 1, sx);
        if (px + 3 <= sx) r += sx - px - 2;
        if (i > 0 and px + 1 == sx) ++r;
        chmax(res, r);
        //branch = "gt";
        if (tx <= px + 1 and px + 1 <= sx) {
          r = f(f, i + 1, px + 1);
          if (i > 0) ++r;
          if (chmax(res, r)) {
            // branch += '+';
          }
        }
      }
//    DUMP(i, px, res, branch);
      memo[i][px] = res;
      return res;
    };
    DUMP(S);
    DUMP(T);
    chmax(ans, f(f, 0, -1));
  }
  return ans;
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
