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

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  static_assert(std::is_invocable_r_v<bool, F, T>);
  assert(std::max(true_x, false_x) <= std::numeric_limits<T>::max() / 2);
  assert(true_x >= -1 and false_x >= -1);  // need floor_div to allow negative.

  while (std::abs(true_x - false_x) > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

auto solve() {
  string a = in, b = in;
  int na = ssize(a), nb = ssize(b);
  bool a_is_smaller = false;
  if (na < nb or (na == nb and a < b)) {
    swap(a, b);
    swap(na, nb);
    a_is_smaller = true;
  }

  vector<int> ca(10, 0), cb(10, 0);
  for (char x: a) ca[x - '0']++;
  for (char x: b) cb[x - '0']++;

  int max_carry = -1;
  string mda, mdb;
  for (int x0 = 1; x0 <= 9; ++x0) {
    if (ca[x0] == 0) continue;
    int y0 = -1;
    for (int j = 10 - x0; j <= 9; ++j) {
      if (cb[j]) {
        y0 = j;
        break;
      }
    }
    if (y0 == -1) continue;
    auto ta = ca, tb = cb;
    string da, db;
    check(--ta[x0] >= 0);
    check(--tb[y0] >= 0);
    da.push_back(x0 + '0');
    db.push_back(y0 + '0');
    int carry = 1;  // streak
    int carried = 1;  // bool
    bool carrying = true;
    for (int i = 1; i < na; ++i) {
      int x = -1, y = -1;

      if (i >= nb) {
        y = 0;
      } else {
        for (int k = 1; k <= 9; ++k) {
          if (ta[k] == 0) continue;
          for (int j = max(10 - (k + carried), 0); j <= 9; ++j) {
            if (tb[j] == 0) continue;
            x = k;
            y = j;
            break;
          }
          if (y != -1) break;
        }
      }
      if (x == -1) {
        for (int j = 9; j >= 1; --j) {
          if (ta[j] == 0) continue;
          x = j;
          break;
        }
      }
      if (y == -1) {
        for (int j = 9; j >= 1; --j) {
          if (tb[j] == 0) continue;
          y = j;
          break;
        }
      }
      check(--ta[x] >= 0);
      da.push_back(x + '0');
      if (y != 0) {
        check(--tb[y] >= 0);
        db.push_back(y + '0');
      }

      carried = (x + y + carried) >= 10;
      if (carrying and carried) {
        ++carry;
      }
      if (not carried) carrying = false;
    }
    if (chmax(max_carry, carry)) {
      swap(mda, da);
      swap(mdb, db);
    }
  }
  if (max_carry == -1) {
    mda = a;
    mdb = b;
  } else {
    reverse(ALL(mda));
    reverse(ALL(mdb));
  }
  if (a_is_smaller) {
    swap(mda, mdb);
  }
  DUMP(max_carry);
  DUMP(mda);
  DUMP(mdb);
  print(mda);
  print(mdb);
  return Void{};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
