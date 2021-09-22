#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

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

inline bool is_palindrome(const string &s) {
  for (int i = 0;; ++i) {
    const int j = ssize(s) - 1 - i;
    if (j <= i) break;
    if (s[i] != s[j]) return false;
  }
  return true;
}

auto solve() -> optional<int> {
  const int K = in;
  const string S = in;
  const int n = ssize(S);
  if (n == 1) {
    if (K != 1) return nullopt;
    return 0;
  }
  if (K == 0) {
    return is_palindrome(S) ? 1 : 0;
  }

  vector<pair<int, int>> intervals = {pair{0, n}};
  int total_cost = 0;

  for (int k = K; k >= 1; --k) {
    const auto&[l0, r0] = intervals[0];
    const int width = r0 - l0;
    if (width == 0) return nullopt;
    const int half_width = width >> 1;
    if (width & 1) {
      vector<int> cc(26, 0);
      int max_count = 0;
      for (auto[l, r]: intervals) {
        int mid = l + half_width;
        char ch = S[mid];
        chmax(max_count, ++cc[ch - 'a']);
      }
      total_cost += ssize(intervals) - max_count;
    }
    if (k > 1) {
      vector<pair<int, int>> children;
      children.reserve(intervals.size() * 2);
      for (auto[l, r]: intervals) {
        int mid = l + half_width;
        children.emplace_back(l, mid);
        children.emplace_back(mid + width % 2, r);
      }
      swap(children, intervals);
    } else { // k == 1
      if (width == 1) break;
      if (width <= 3) return nullopt;
      int min_extra_cost = kBig;
      string level0;
      level0.reserve(half_width);
      REP(j, half_width) {
        vector<int> cc(26, 0);
        for (const auto&[l, r]: intervals) {
          const int mid = l + half_width;
          const char ch1 = S[mid - 1 - j];
          const char ch2 = S[mid + width % 2 + j];
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
        level0.push_back(tch);
        const int c1 = 2 * ssize(intervals) - top2[0];
        total_cost += c1;
        if (half_width % 2 == 0 or j != half_width / 2) {
          const int c2 = 2 * ssize(intervals) - top2[1];
          chmin(min_extra_cost, c2 - c1);
        }
      }
      if (is_palindrome(level0)) {
        total_cost += min_extra_cost;
      }
    }
  }
  return total_cost;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  if (not ans) {
    cout << "impossible\n";
  } else {
    cout << *ans << "\n";
  }
}
