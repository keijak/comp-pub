#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
const i64 BIG = 1e15;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<pair<i64, i64>> xy(N);
  vector<pair<i64, i64>> uv(N);
  i64 mau = -BIG, mav = -BIG, miu = BIG, miv = BIG;
  int maui = -1, mavi = -1, miui = -1, mivi = -1;
  REP(i, N) {
    i64 x, y;
    cin >> x >> y;
    i64 u = x - y, v = x + y;
    xy[i] = {x, y};
    uv[i] = {u, v};
    if (chmax(mau, u)) maui = i;
    if (chmin(miu, u)) miui = i;
    if (chmax(mav, v)) mavi = i;
    if (chmin(miv, v)) mivi = i;
  }
  REP(_, Q) {
    int q;
    cin >> q;
    --q;
    const auto &[u, v] = uv[q];
    i64 d = -1;
    int j = -1;
    if (chmax(d, abs(mau - u))) j = maui;
    if (chmax(d, abs(miu - u))) j = miui;
    if (chmax(d, abs(mav - v))) j = mavi;
    if (chmax(d, abs(miv - v))) j = mivi;
    assert(j != -1);
    const auto &[x1, y1] = xy[q];
    const auto &[x2, y2] = xy[j];
    cout << abs(x1 - x2) + abs(y1 - y2) << "\n";
  }
}
