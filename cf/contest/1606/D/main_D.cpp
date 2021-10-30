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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
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
      static_assert(std::numeric_limits<T>::max() != T());  // max must be defined
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

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() -> void {
  int n = in, m = in;
  auto a = vector(n, vector(m, 0));
  REP(i, n) REP(j, m) a[i][j] = int(in);
  auto lma = vector(n, vector(m + 1, (int) -kBig));
  auto lmi = vector(n, vector(m + 1, (int) kBig));
  auto rma = vector(n, vector(m + 1, (int) -kBig));
  auto rmi = vector(n, vector(m + 1, (int) kBig));

  REP(j, m) {
    REP(i, n) {
      lma[i][j + 1] = max(lma[i][j], a[i][j]);
      lmi[i][j + 1] = min(lmi[i][j], a[i][j]);
    }
  }
  for (int j = m - 1; j >= 0; --j) {
    REP(i, n) {
      rma[i][j] = max(rma[i][j + 1], a[i][j]);
      rmi[i][j] = min(rmi[i][j + 1], a[i][j]);
    }
  }

  auto f = [&](int j) -> bool {
    MinHeap<pair<int, int>> left_min;
    priority_queue<pair<int, int>> left_max;
    MinHeap<pair<int, int>> right_min;
    priority_queue<pair<int, int>> right_max;
    REP(i, n) {
      left_min.emplace(lmi[i][j], i);
      left_max.emplace(lma[i][j], i);
      right_min.emplace(rmi[i][j], i);
      right_max.emplace(rma[i][j], i);
    }
    int left_red_min = kBig;
    int left_blue_max = -kBig;
    int right_blue_min = kBig;
    int right_red_max = -kBig;
    queue<pair<int, int>> cq;
    cq.emplace(left_min.top().second, -1);  // blue
    cq.emplace(right_max.top().second, -1);  // blue
    cq.emplace(right_min.top().second, 1);  // red
    cq.emplace(left_max.top().second, 1);  // red
    left_min.pop();
    left_max.pop();
    right_min.pop();
    right_max.pop();
    vector<int> color(n, 0); // -1:blue, 0:unknown, 1:red
    while (not cq.empty()) {
      auto[i, co] = cq.front();
      cq.pop();
      if (color[i] != 0) {
        if (color[i] != co) return false;  // contradiction!
        continue;
      }
      color[i] = co;
      if (co == 1) {
        if (lmi[i][j] <= left_blue_max) return false;
        chmin(left_red_min, lmi[i][j]);
        while (not left_max.empty() and left_max.top().first >= left_red_min) {
          int x = left_max.top().second;
          left_max.pop();
          cq.emplace(x, 1);
        }
        if (rma[i][j] >= right_blue_min) return false;
        chmax(right_red_max, rma[i][j]);
        while (not right_min.empty() and right_min.top().first <= right_red_max) {
          int x = right_min.top().second;
          right_min.pop();
          cq.emplace(x, 1);
        }
      } else {
        if (lma[i][j] >= left_red_min) return false;
        chmax(left_blue_max, lma[i][j]);
        while (not left_min.empty() and left_min.top().first <= left_blue_max) {
          int x = left_min.top().second;
          left_min.pop();
          cq.emplace(x, -1);
        }
        if (rmi[i][j] <= right_red_max) return false;
        chmin(right_blue_min, rmi[i][j]);
        while (not right_max.empty() and right_max.top().first >= right_blue_min) {
          int x = right_max.top().second;
          right_max.pop();
          cq.emplace(x, -1);
        }
      }
    }
    print("YES");
    REP(i, n) {
      if (color[i] == -1) cout << "B";
      else cout << "R";
    }
    cout << " " << j << endl;
    return true;
  };

  for (int j = 1; j < m; ++j) {
    if (f(j)) return;
  }
  print("NO");
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    solve();
  }
}
