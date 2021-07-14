#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const Int BIG = Int(1e19);

string solve() {
  string s;
  i64 k_;
  cin >> s >> k_;
  Int K = k_;
  const int n = s.size();
  auto dp = vector(n + 1, Int(0));
  auto dp_cum = vector(n + 1, Int(0));
  auto dp_sum = vector(n + 2, Int(0));
  vector<deque<int>> nex(26);
  for (int i = n - 1; i >= 0; --i) {
    auto &q = nex[s[i] - 'a'];
    int j = n;
    if (not q.empty()) {
      j = q.front();
    } else {
      j = n;
      dp[i] += 1;
    }
    q.push_front(i);

    dp[i] += dp_sum[i + 1] - dp_sum[j + 1];
    dp_cum[i] = dp_cum[j] + dp[i];
    dp_sum[i] = dp_sum[i + 1] + dp[i];

    // truncate
    chmin(dp[i], BIG);
    chmin(dp_cum[i], BIG);
    chmin(dp_sum[i], BIG);
  }
  Int total = 0;
  REP(i, n) { total += dp[i]; }
  if (total < K) {
    return "Eel";
  }

  string ans;
  int pos = 0;
  while (K > 0 and pos < n) {
    bool found = false;
    for (char ch = 'a'; ch <= 'z'; ++ch) {
      auto it = lower_bound(ALL(nex[ch - 'a']), pos);
      if (it == nex[ch - 'a'].end()) continue;
      int j = *it;
      if (dp_cum[j] < K) {
        K -= dp_cum[j];
        continue;
      }
      --K;
      ans.push_back(ch);
      pos = j + 1;
      found = true;
      DUMP(ans, pos, K);
      break;
    }
    if (not found) break;
  }
  if (K != 0) {
    return "Eel";
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
