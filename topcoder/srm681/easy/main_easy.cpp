#include "gtest/gtest.h"

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

#ifdef MY_DEBUG
//#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
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

class FleetFunding {
 public:
  int maxShips(int m, vector<int> k, vector<int> a, vector<int> b) {
    int n = k.size();
    REP(i, n) --a[i];
    vector<int> idx(n);
    REP(i, n) idx[i] = i;
    sort(ALL(idx), [&](int i, int j) {
      return b[i] < b[j];
    });

    return bisect<int>(0, 1e8, [&](int x) {
      auto supply = k;
      REP(i, m) {
        int norma = x;
        for (int j: idx) {
          if (norma == 0) break;
          if (supply[j] > 0 and a[j] <= i and i < b[j]) {
            int y = min<int>(norma, supply[j]);
            supply[j] -= y;
            norma -= y;
          }
        }
        if (norma > 0) return false;
      }
      return true;
    });
  }
};

TEST(FleetFunding, Test) {
  FleetFunding ff;
  EXPECT_EQ(ff.maxShips(3, {2, 2, 2}, {1, 2, 1}, {3, 3, 2}), 2);
  EXPECT_EQ(ff.maxShips(
      1, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}), 55);
  EXPECT_EQ(ff.maxShips(
      10,
      {2, 4, 6, 8, 10, 1, 3, 5, 7, 9},
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), 1);
  EXPECT_EQ(ff.maxShips(
      84457,
      {374778, 169109, 529984, 498638, 29462, 465769, 29285, 394948, 307944, 459189,
       349316, 766627, 686851, 404285, 850199, 359378, 3014, 248257, 558856, 27393, 32370,
       574862, 337134, 965567, 32223, 935678, 389898, 973497, 990575, 483190, 122899,
       857994, 991217, 149619, 415870},
      {92, 4997, 11, 28545, 933, 210, 2, 124, 114, 4513, 32959, 1, 57, 17, 13, 133, 1843, 41851,
       3, 9296, 9757, 28, 3, 769, 10, 11102, 683, 6173, 11821, 3982, 214, 2, 4304, 439, 1998},
      {81034, 54474, 70239, 79597, 47317, 82938, 83883, 74652, 84033, 84422, 84456, 84457,
       81095, 83743, 79210, 84255, 84455, 45596, 84456, 82959, 67062, 80330, 44090, 84445,
       84454, 84450, 45931, 77190, 83025, 83567, 83639, 84333, 83355, 70982, 84456}),
            186);

  EXPECT_EQ(ff.maxShips(
      2,
      {1000000, 1000000, 1000000, 1000000, 1000000},
      {1, 1, 1, 2, 2},
      {1, 1, 2, 2, 2}), 2500000);
}
