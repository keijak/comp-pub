#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

i64 solve() {
  int n;
  cin >> n;
  MinHeap<tuple<i64, i64, i64>> heap;
  map<pair<i64, i64>, i64> mincost;

  auto push = [&](i64 cost, i64 num, i64 c) -> bool {
    auto it = mincost.find({num, c});
    if (it != mincost.end() and it->second <= cost) return false;
    if (it == mincost.end()) {
      mincost[pair(num, c)] = cost;
    } else {
      it->second = cost;
    }
    heap.push({cost, num, c});
    return true;
  };
  push(0, n, 0);
  while (not heap.empty()) {
    auto [cost, num, mcount] = heap.top();
    heap.pop();
    if (mincost[{num, mcount}] != cost) continue;
    if (num == 1) return cost;
    if (num % 3 == 0) {
      push(cost + 5, num / 3, 0);
    }
    if (num - 4 >= 1 and mcount <= 6) {
      push(cost + 2, num - 4, mcount + 1);
    }
  }
  return 0;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
