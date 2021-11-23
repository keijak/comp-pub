#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
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

const Int L = 1'000'000;

auto solve(Int n) {
  vector<Int> t;
  Int s = 0;
  map<Int, int> tm;
  for (int i = 1;; ++i) {
    s += i;
    if (s > L) break;
    t.push_back(s);
    tm[s] = i - 1;
  }
  string ans;
  REP(i, ssize(t)) {
    if (t[i] == n) {
      ans += string(i + 1, '7');
      return ans;
    }
  }
  REP(i, ssize(t)) {
    REP(j, ssize(t)) {
      if (t[i] + t[j] == n) {
        ans += string(i + 1, '7');
        ans += "1";
        ans += string(j + 1, '7');
        return ans;
      }
    }
  }
  REP(i, ssize(t)) {
    REP(j, ssize(t)) {
      auto it = tm.find(n - t[i] - t[j]);
      if (it != tm.end()) {
        int k = it->second;
        check(t[k] + t[i] + t[j] == n);
        char d1 = '1', d2 = '6';
        if ((j + 1) % 6 == 5) {
          d2 = '1';
        }
        ans += string(i + 1, '7');
        ans += d1;
        ans += string(j + 1, '7');
        ans += d2;
        ans += string(k + 1, '7');
        return ans;
      }
    }
  }
  check(false);
}

void test(Int n, const string &a) {
  int cnt = 0;
  REP(r, 1, ssize(a) + 1) {
    for (int l = r - 1; l >= 0; --l) {
      Int s = 0;
      for (int i = l; i < r; ++i) {
        int d = int(a[i] - '0');
        s *= 10;
        s += d;
        s %= 7;
      }
      if (s % 7 == 0) {
        ++cnt;
      }
    }
  }
  check(cnt == n);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    Int n = in;
    auto ans = solve(n);
    //test(n, ans);
    print(ans);
  }
}
