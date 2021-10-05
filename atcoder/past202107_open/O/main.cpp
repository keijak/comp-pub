#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() -> i64 {
  const i64 n = in;
  vector<pair<i64, i64>> queries(n);
  vector<i64> cuma(n + 1);
  vector<pair<i64, i64>> maxb(n + 1);
  maxb[0] = {0, -1};
  REP(i, n) {
    i64 a = in, b = in;
    queries[i] = {a, b};
    cuma[i + 1] = cuma[i] + a;
    if (b > maxb[i].first) {
      maxb[i + 1] = {b, i};
    } else {
      maxb[i + 1] = maxb[i];
    }
  }
  DUMP(maxb[n]);
  REP(i, n) {
    if (cuma[i + 1] < queries[i].second) {
      return -1;
    }
  }

  vector<pair<optional<pair<i64, i64>>, bool>> memo(n + 1, {nullopt, false});
  auto earliest_buy = [&](auto &f, i64 k) -> optional<pair<i64, i64>> {
    if (memo[k].second) return memo[k].first;
    memo[k].second = true;
    if (k == 0) {
      memo[k].first = pair{-1, 0LL};
      return memo[k].first;
    }

    i64 b;
    i64 i;
    tie(b, i) = maxb[k];

    auto sub0 = f(f, i);
    if (not sub0) return nullopt;
    i64 j0;
    i64 amt0;
    tie(j0, amt0) = sub0.value();

    i64 ok = i, ng = -1;
    i64 ok_amt = amt0 + cuma[i + 1] - cuma[j0 + 1];
    if (ok_amt < b) return nullopt;

    while (ok - ng > 1LL) {
      i64 mid = (ok + ng) / 2;
      auto sub = f(f, mid);
      if (not sub) {
        ng = mid;
        continue;
      }
      i64 j;
      i64 amt;
      tie(j, amt) = sub.value();
      i64 amt2 = amt + cuma[mid + 1] - cuma[j + 1];
      if (amt2 >= b) {
        ok = mid;
        ok_amt = amt2;
      } else {
        ng = mid;
      }
    }
    check(ok_amt >= b);
    memo[k].first = pair{ok, ok_amt - b};
    return memo[k].first;
  };
  auto eb = earliest_buy(earliest_buy, n);
  if (not eb) {
    return -1;
  }
  auto[day, amt] = eb.value();
  return amt + cuma[n] - cuma[day + 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
