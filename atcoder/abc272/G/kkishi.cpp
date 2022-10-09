#include <bits/stdc++.h>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// big = 2305843009213693951 = 2^61-1 ~= 2.3*10^18
const int64_t big = std::numeric_limits<int64_t>::max() / 4;

#endif  // CONSTANTS_H_
#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

template <typename T, typename = void>
struct is_dereferenceable : std::false_type {};
template <typename T>
struct is_dereferenceable<T, std::void_t<decltype(*std::declval<T>())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = void>
struct is_applicable : std::false_type {};
template <typename T>
struct is_applicable<T, std::void_t<decltype(std::tuple_size<T>::value)>>
    : std::true_type {};

#endif  // TYPE_TRAITS_H

template <typename T, typename... Ts>
void debug(std::ostream& os, const T& value, const Ts&... args);
template <typename T>
void debug(std::ostream& os, const T& v) {
  if constexpr (std::is_same<int64_t, std::decay_t<T>>::value) {
    if (v == big) {
      os << "big";
    } else {
      os << v;
    }
  } else if constexpr (std::is_same<char*, std::decay_t<T>>::value ||
                       std::is_same<std::string, T>::value) {
    os << v;
  } else if constexpr (is_dereferenceable<T>::value) {
    os << "{";
    if (v) {
      debug(os, *v);
    } else {
      os << "nil";
    }
    os << "}";
  } else if constexpr (is_iterable<T>::value) {
    os << "{";
    for (auto it = std::begin(v); it != std::end(v); ++it) {
      if (it != std::begin(v)) os << ", ";
      debug(os, *it);
    }
    os << "}";
  } else if constexpr (is_applicable<T>::value) {
    os << "{";
    std::apply([&os](const auto&... args) { debug(os, args...); }, v);
    os << "}";
  } else {
    os << v;
  }
}
template <typename T, typename... Ts>
void debug(std::ostream& os, const T& value, const Ts&... args) {
  debug(os, value);
  os << ", ";
  debug(os, args...);
}
#if DEBUG
#define dbg(...)                             \
  do {                                       \
    std::cerr << #__VA_ARGS__ << ": ";       \
    debug(std::cerr, __VA_ARGS__);           \
    std::cerr << " (L" << __LINE__ << ")\n"; \
  } while (0)
#else
#define dbg(...)
#endif

#endif  // DEBUG_H_
#ifndef FIX_H_
#define FIX_H_

template <class F>
struct FixPoint {
  F f;
  template <class... Args>
  decltype(auto) operator()(Args&&... args) const {
    return f(std::ref(*this), std::forward<Args>(args)...);
  }
};
template <class F>
FixPoint<std::decay_t<F>> Fix(F&& f) {
  return {std::forward<F>(f)};
}

#endif  // FIX_H_
#ifndef IO_H_
#define IO_H

void read_from_cin() {}
template <typename T, typename... Ts>
void read_from_cin(T& value, Ts&... args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define rd(type, ...) \
  type __VA_ARGS__;   \
  read_from_cin(__VA_ARGS__);
#define ints(...) rd(int, __VA_ARGS__);
#define strings(...) rd(string, __VA_ARGS__);

const char *yes_str = "Yes", *no_str = "No";

template <typename T>
void write_to_cout(const T& value) {
  if constexpr (std::is_same<T, bool>::value) {
    std::cout << (value ? yes_str : no_str);
  } else if constexpr (is_iterable<T>::value &&
                       !std::is_same<T, std::string>::value) {
    for (auto it = std::begin(value); it != std::end(value); ++it) {
      if (it != std::begin(value)) std::cout << " ";
      std::cout << *it;
    }
  } else {
    std::cout << value;
  }
}
template <typename T, typename... Ts>
void write_to_cout(const T& value, const Ts&... args) {
  write_to_cout(value);
  std::cout << ' ';
  write_to_cout(args...);
}
#define wt(...)                 \
  do {                          \
    write_to_cout(__VA_ARGS__); \
    cout << '\n';               \
  } while (0)

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
  for (T& vi : v) is >> vi;
  return is;
}

template <typename T, typename U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& p) {
  is >> p.first >> p.second;
  return is;
}

#endif  // IO_H_
#ifndef MACROS_H_
#define MACROS_H_

#define all(x) (x).begin(), (x).end()
#define eb(...) emplace_back(__VA_ARGS__)
#define pb(...) push_back(__VA_ARGS__)

#define dispatch(_1, _2, _3, name, ...) name

#define as_i64(x)                                                          \
  (                                                                        \
      [] {                                                                 \
        static_assert(                                                     \
            std::is_integral<                                              \
                typename std::remove_reference<decltype(x)>::type>::value, \
            "rep macro supports std integral types only");                 \
      },                                                                   \
      static_cast<int64_t>(x))

#define rep3(i, a, b) for (int64_t i = as_i64(a); i < as_i64(b); ++i)
#define rep2(i, n) rep3(i, 0, n)
#define rep1(n) rep2(_loop_variable_, n)
#define rep(...) dispatch(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)

#define rrep3(i, a, b) for (int64_t i = as_i64(b) - 1; i >= as_i64(a); --i)
#define rrep2(i, n) rrep3(i, 0, n)
#define rrep1(n) rrep2(_loop_variable_, n)
#define rrep(...) dispatch(__VA_ARGS__, rrep3, rrep2, rrep1)(__VA_ARGS__)

#define each3(k, v, c) for (auto&& [k, v] : c)
#define each2(e, c) for (auto&& e : c)
#define each(...) dispatch(__VA_ARGS__, each3, each2)(__VA_ARGS__)

template <typename T, typename U>
bool chmax(T& a, U b) {
  if (a < b) {
    a = b;
    return true;
  }
  return false;
}

template <typename T, typename U>
bool chmin(T& a, U b) {
  if (a > b) {
    a = b;
    return true;
  }
  return false;
}

template <typename T, typename U>
auto max(T a, U b) {
  return a > b ? a : b;
}

template <typename T, typename U>
auto min(T a, U b) {
  return a < b ? a : b;
}

template <typename T>
auto max(const T& v) {
  return *std::max_element(v.begin(), v.end());
}

template <typename T>
auto min(const T& v) {
  return *std::min_element(v.begin(), v.end());
}

template <typename T>
int64_t sz(const T& v) {
  return std::size(v);
}

template <typename T>
int64_t popcount(T i) {
  return std::bitset<std::numeric_limits<T>::digits>(i).count();
}

template <typename T>
bool hasbit(T s, int i) {
  return std::bitset<std::numeric_limits<T>::digits>(s)[i];
}

template <typename T, typename U>
auto div_floor(T n, U d) {
  if (d < 0) {
    n = -n;
    d = -d;
  }
  if (n < 0) {
    return -((-n + d - 1) / d);
  }
  return n / d;
};

template <typename T, typename U>
auto div_ceil(T n, U d) {
  if (d < 0) {
    n = -n;
    d = -d;
  }
  if (n < 0) {
    return -(-n / d);
  }
  return (n + d - 1) / d;
}

template <typename T>
bool even(T x) {
  return x % 2 == 0;
}

std::array<std::pair<int64_t, int64_t>, 4> adjacent(int64_t i, int64_t j) {
  return {{{i + 1, j}, {i, j + 1}, {i - 1, j}, {i, j - 1}}};
}

bool inside(int64_t i, int64_t j, int64_t I, int64_t J) {
  return 0 <= i && i < I && 0 <= j && j < J;
}

template <typename T>
void sort(T& v) {
  return std::sort(v.begin(), v.end());
}

template <typename T, typename Compare>
void sort(T& v, Compare comp) {
  return std::sort(v.begin(), v.end(), comp);
}

template <typename T>
void reverse(T& v) {
  return std::reverse(v.begin(), v.end());
}

template <typename T>
typename T::value_type accumulate(const T& v) {
  return std::accumulate(v.begin(), v.end(), typename T::value_type());
}

using i64 = int64_t;
using i32 = int32_t;

template <typename T>
using low_priority_queue =
    std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename T>
using V = std::vector<T>;
template <typename T>
using VV = V<V<T>>;

#endif  // MACROS_H_

void Main();

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout << std::fixed << std::setprecision(20);
  Main();
  return 0;
}

using namespace std;

#define int i64

template <typename T>
std::vector<T> Divisors(T x) {
  std::vector<T> h, t;
  for (T i = 1; i * i <= x; ++i) {
    if (x % i == 0) {
      h.push_back(i);
      if (T j = x / i; j != i) t.push_back(j);
    }
  }
  copy(t.rbegin(), t.rend(), back_inserter(h));
  return h;
}

class Rand {
 public:
  Rand() : generator_(device_()) {}
  int64_t Int(int64_t a, int64_t b) {
    return std::uniform_int_distribution<int64_t>(a, b)(generator_);
  }
  double Double(double a, double b) {
    return std::uniform_real_distribution<double>(a, b)(generator_);
  }
  char Alpha() { return Int('a', 'z'); }
  template <typename T>
  void Shuffle(std::vector<T>& v) {
    for (size_t i = 0; i < v.size() - 1; ++i) {
      std::swap(v[i], v[Int(i, v.size() - 1)]);
    }
  }
  std::vector<int64_t> Permutation(int64_t n) {
    std::vector<int64_t> v(n);
    std::iota(v.begin(), v.end(), 1);
    Shuffle(v);
    return v;
  }
  std::vector<std::pair<int64_t, int64_t>> Tree(int64_t n) {
    std::vector<std::pair<int64_t, int64_t>> v;
    for (int64_t i = 1; i < n; ++i) {
      int64_t j = Int(0, i - 1);
      v.emplace_back(j + 1, i + 1);
    }
    return v;
  }

 private:
  std::random_device device_;
  std::mt19937 generator_;
};

void Main() {
  ints(n);
  V<int> a(n);
  cin >> a;
  Rand r;
  int ans = -1;
  rep(100) {
    int i = r.Int(0, n - 1);
    int j = r.Int(0, n - 1);
    if (i == j) continue;
    each(d, Divisors(abs(a[i] - a[j]))) {
      if (3 <= d) {
        int cnt = 0;
        rep(k, n) if (a[k] % d == a[i] % d)++ cnt;
        if (cnt * 2 > n) ans = d;
      }
    }
  }
  wt(ans);
}
