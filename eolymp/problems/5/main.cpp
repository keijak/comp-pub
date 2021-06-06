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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  int k;
  cin >> k;
  const i64 LIMIT = 1e15;
  const vector<int> primes = {2, 3, 5, 7, 11};

  MinHeap<pair<i64, map<int, int>>> q;
  set<i64> visited;
  q.push({1LL, {}});
  while (not q.empty()) {
    auto [val, fac] = q.top();
    q.pop();
    {
      i64 r = 1;
      for (auto [k, v] : fac) {
        r *= (v + 1);
      }
      i64 x = (r + 1) / 2;
      if (x == k) {
        // int max_prime = 1;
        // if (not fac.empty()) {
        //   max_prime = fac.rbegin()->first;
        // }
        cout << val << endl;
        exit(0);
      }
    }

    for (auto p : primes) {
      i64 val2 = val * p;
      if (val2 < LIMIT and visited.count(val2) == 0) {
        visited.insert(val2);
        auto fac2 = fac;
        fac2[p] += 1;
        q.push(pair(val2, fac2));
      }
    }
  }
}
