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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

auto solve() -> optional<i64> {
  const int K = in;
  const string S = in;
  const int n = ssize(S);
  if (n == 1) {
    if (K != 1) return nullopt;
    return 0;
  }
  if (K == 0) {
    auto R = S;
    reverse(ALL(R));
    if (S == R) return 1;
    return 0;
  }
  if (K == 1) {
    if (n <= 3) return nullopt;
    int m = n / 2;
    string L = S.substr(0, m);
    string R = S.substr(m + (n % 2), m);
    bool eq = (L == R);
    reverse(ALL(R));
    if (L == R) {
      if (eq) return 2;
      return 0;
    }
    int cnt = 0;
    REP(i, m) {
      if (L[i] == R[i]) continue;
      ++cnt;
    }
    return cnt;
  }

  auto f = [&](auto &f, int k, const vector<pair<int, int>> &intervals) -> optional<i64> {
    assert(not intervals.empty());
    assert(k > 0);
    auto[l0, r0] = intervals[0];
    const i64 width = r0 - l0;
    if (width == 0) return nullopt;
    i64 center_cost = 0;
    if (width % 2 == 1) {
      vector<int> cc(26, 0);
      int max_count = 0;
      for (auto[l, r]: intervals) {
        int mid = l + width / 2;
        char ch = S[mid];
        chmax(max_count, ++cc[ch - 'a']);
      }
      center_cost = ssize(intervals) - max_count;
    }
    if (k == 1) {
      if (width == 1) return center_cost;
      i64 cost = center_cost;
      i64 mindiff = kBig;
      string x;
      REP(j, width / 2) {
        vector<int> cc(26, 0);
        for (auto[l, r]: intervals) {
          int mid = l + width / 2;
          char ch1 = S[mid - 1 - j];
          char ch2 = S[mid + width % 2 + j];
          ++cc[ch1 - 'a'];
          ++cc[ch2 - 'a'];
        }
        array<int, 2> top2 = {0, 0};
        char tch = '#';
        REP(c, 26) {
          if (cc[c] > top2[0]) {
            top2[1] = top2[0];
            top2[0] = cc[c];
            tch = 'a' + c;
          } else if (cc[c] > top2[1]) {
            top2[1] = cc[c];
          }
        }
        x.push_back(tch);
        const i64 c1 = 2 * ssize(intervals) - top2[0];
        cost += c1;
        if (width / 2 % 2 == 0 or j != (width / 2) / 2) {
          const i64 c2 = 2 * ssize(intervals) - top2[1];
          chmin(mindiff, c2 - c1);
        }
      }
      string rx = x;
      reverse(ALL(rx));
      if (rx == x) {
        if (width <= 3) return nullopt;
        cost += mindiff;
      }
      return cost;
    }
    vector<pair<int, int>> children;
    for (auto[l, r]: intervals) {
      int mid = l + width / 2;
      children.emplace_back(l, mid);
      children.emplace_back(mid + width % 2, r);
    }
    auto sub = f(f, k - 1, children);
    if (not sub) return nullopt;
    i64 ret = *sub + center_cost;
    return ret;
  };
  vector<pair<int, int>> v;
  v.emplace_back(0, n / 2);
  v.emplace_back(n / 2 + n % 2, n);
  return f(f, K - 1, v);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print("impossible");
    } else {
      print(*ans);
    }
  }
}
