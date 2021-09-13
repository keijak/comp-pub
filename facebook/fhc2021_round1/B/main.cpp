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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve(int n, int m, int A, int B) -> optional<vector<vector<int>>> {
  if (A < n + m - 1 or B < n + m - 1) return nullopt;
  if (A > 1000LL * (n + m - 1) or B > 1000LL * (n + m - 1)) return nullopt;
  auto ans = vector(n, vector(m, 1000));
  i64 asum = 0, bsum = 0;
  vector<pair<int, int>> ai, bi;
  REP(i, n) {
    ans[i][m - 1] = 1;
    bi.emplace_back(i, m - 1);
    ++bsum;
  }
  for (int j = m - 2; j >= 0; --j) {
    ans[n - 1][j] = 1;
    bi.emplace_back(n - 1, j);
    ++bsum;
  }
  {
    int i = n - 1, j = m - 1, c = 0;
    while (i >= 0 and j >= 0) {
      ans[i][j] = 1;
      ai.emplace_back(i, j);
      ++asum;
      if (c % 2 == 0) {
        --j;
      } else {
        --i;
      }
      ++c;
    }
    if (i < 0) {
      while (--j >= 0) {
        ans[0][j] = 1;
        ai.emplace_back(0, j);
        ++asum;
      }
    } else {
      while (--i >= 0) {
        ans[i][0] = 1;
        ai.emplace_back(i, 0);
        ++asum;
      }
    }
    reverse(ALL(ai));
  }

//  DUMP(asum, bsum);
//  DUMP(ai);
//  DUMP(bi);
  if (asum > A or bsum > B) return nullopt;
  REP(i, ai.size()) {
    auto[x, y] = ai[i];
    int d = A - asum;
    chmin(d, 999);
    ans[x][y] += d;
    asum += d;
    if (i == ssize(ai) - 1 or i == ssize(ai) - 2) {
      bsum += d;
    }
  }
  if (asum > A or bsum > B) return nullopt;
//  DUMP(asum, A, bsum, B);
  REP(i, bi.size()) {
    int ri = ssize(bi) - 1 - i;
    if (ri < i) break;
    for (int j: {i, ri}) {
      auto[x, y] = bi[j];
      int d = B - bsum;
      chmin(d, 999);
      ans[x][y] += d;
      bsum += d;
      if (i == ri) break;
    }
  }
  if (asum != A or bsum != B) return nullopt;
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    //DUMP(test_case);
    const int n = in, m = in, A = in, B = in;
    auto ans = solve(n, m, A, B);
    if (n <= 3 and m <= 3) {
      DUMP(n, m, A, B);
      if (not ans) {
        DUMP("Impossible");
      } else {
        REP(i, 2) DUMP(ans.value()[i]);
      }
    }
    cout << "Case #" << (test_case + 1) << ": ";
    if (not ans) {
      print("Impossible");
    } else {
      print("Possible");
      auto g = move(*ans);
      for (const auto &row: g) {
        print_seq(row);
      }
    }
  }
}
