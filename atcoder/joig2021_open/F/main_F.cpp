#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};

using namespace std;

const int A = 256;

auto solve() {
  const int H = in, W = in;
  const Int S = in;
  auto g = vector(H, vector(W, -1));
  REP(i, H) REP(j, W) cin >> g[i][j], --g[i][j];
  REP(i, H) { DUMP(i, g[i]); }
  vector<int> xmin(A, kBigVal<int>), xmax(A, -1);
  vector<int> ymin(A, kBigVal<int>), ymax(A, -1);
  REP(i, H) {
    REP(j, W) {
      int color = g[i][j];
      chmin(xmin[color], i);
      chmax(xmax[color], i);
      chmin(ymin[color], j);
      chmax(ymax[color], j);
    }
  }
  vector<int> xs, ys;
  int num_colors = 0;
  REP(i, A) {
    if (xmax[i] == -1) continue;
    ++num_colors;
    xs.push_back(xmin[i]);
    xs.push_back(xmax[i]);
    ys.push_back(ymin[i]);
    ys.push_back(ymax[i]);
  }
  DUMP(num_colors);
  Int ans = 0;
  Compressed<int> xc(xs), yc(ys);
  const int R = xc.size();
  DUMP(xc.values);
  REP(i_lo, R) {
    int x_lo = xc.value(i_lo);
    REP(i_hi, i_lo, R) {
      int x_hi = xc.value(i_hi);
      Int wmax = S / (x_hi + 1 - x_lo);
      DUMP(x_lo, x_hi, S, wmax);
      if (wmax == 0) continue;
      vector<tuple<int, bool, int>> events;
      REP(j, A) {
        if (xmax[j] == -1 or xmin[j] < x_lo or xmax[j] > x_hi) continue;
        if (ymax[j] + 1 - ymin[j] > wmax) {
          DUMP("too wide", j);
          continue;
        }
        events.emplace_back(ymin[j], false, j);
        events.emplace_back(ymax[j], true, j);
      }
      DUMP(events);
      if (events.empty()) continue;
      sort(ALL(events));
      deque<pair<int, int>> opens;
      bitset<256> open, valid;
      int num_valid = 0;
      {
        auto[y0, closing, j0] = events[0];
        assert(not closing);
        opens.push_back({y0, j0});
        open.set(j0, true);
      }
      Int max_count = 0;
      REP(e, 1, events.size()) {
        auto[y, closing, j] = events[e];
        if (closing) {
          while (not opens.empty() and opens.front().first <= y - wmax) {
            auto[yl, jl] = opens.front();
            open.set(jl, false);
            if (valid[jl]) {
              valid.set(jl, false);
              --num_valid;
            }
            opens.pop_front();
          }
          if (open[j]) {
            open.set(j, false);
            valid.set(j, true);
            ++num_valid;
          }
          chmax(max_count, num_valid);
        } else {
          opens.push_back({y, j});
          open.set(j, true);
        }
      }
      chmax(max_count, num_valid);
      chmax(ans, max_count);
      DUMP(x_lo, x_hi, max_count, ans);
    }
  }
  return num_colors - ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
