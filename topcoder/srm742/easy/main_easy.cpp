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
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

class ResistorFactory {
 public:
  vector<int> construct(Int ohms) {
    Int target = ohms;
    vector<array<int, 3>> commands;
    Int uv = 1000000000LL;
    int ui = 0;
    while (uv * 2 <= ohms) {
      commands.push_back({ui, ui, 0});
      uv *= 2;
      ++ui;
    }
    Int v = uv;
    int cur = -1;
    for (int i = ui; i >= 0; --i, v /= 2) {
      if (v <= ohms) {
        ohms -= v;
        if (cur >= 0) {
          commands.push_back({i, cur, 0});
        }
        cur = int(commands.size());
      }
    }
    v <<= 28;
    ohms <<= 28;
    int i = 0;
    for (; ohms > 0 and ssize(commands) * 3 <= 1000; v /= 2) {
      commands.push_back({i, i, 1});
      i = int(commands.size());
      if (v <= ohms) {
        ohms -= v;
        if (cur >= 0) {
          commands.push_back({i, cur, 0});
          cur = int(commands.size());
        } else {
          cur = i;
        }
      }
    }

//    {
//      vector<Real> prod = {1000000000LL};
//      REP(x, cur) {
//        auto[i, j, op] = commands[x];
//        if (op == 0) {
//          prod.push_back(prod[i] + prod[j]);
//        } else {
//          prod.push_back(prod[i] * prod[j] / (prod[i] + prod[j]));
//        }
//      }
//      DUMP(prod);
//      DUMP(prod.back(), target);
//    }

    vector<int> ret;
    REP(j, cur) {
      const auto &a = commands[j];
      REP(i, 3) ret.push_back(a[i]);
    }
    return ret;
  }
};

TEST(TopCoder, SRM) {
  ResistorFactory z;
  EXPECT_EQ(z.construct(3000000000LL),
            (vector{0, 0, 0, 0, 1, 0}));
  DUMP(z.construct(333333333LL));
}
