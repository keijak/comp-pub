#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using namespace std;

template <unsigned M>
struct ModInt {
  ModInt(long long val = 0) : _v(0) {
    if (val < 0) {
      long long k = (abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static int mod() { return M; }
  static unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  ModInt operator-() const { return ModInt(-_v); }
  ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long)(_v)*a._v) % M;
    return *this;
  }

  friend ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
  }
  friend bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  unsigned _v;  // raw value
};
const unsigned MOD = int(1e9) + 7;
using Mint = ModInt<MOD>;

struct BearCavalry {
  int countAssignments(vector<int> warriors, vector<int> horses) {
    int n = warriors.size();
    sort(warriors.begin() + 1, warriors.end());
    sort(ALL(horses));
    auto f = [&]() -> Mint {
      int lim = warriors[0] * horses[0];
      int i = 0;
      Mint res = 1;
      for (int j = 0; j < n - 1; ++j) {
        int x = horses[n - 1 - j];
        while (i + 1 < n and warriors[i + 1] * x < lim) {
          ++i;
        }
        if (i <= j) return 0;
        res *= i - j;
      }
      return res;
    };

    Mint ans = 0;
    for (int i = 0; i < n; ++i) {
      swap(horses[0], horses[i]);
      Mint a = f();
      ans += a;
    }
    return int(ans.val());
  }
};

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
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  BearCavalry s;
  int a;

  a = s.countAssignments({5, 8, 4, 8}, {19, 40, 25, 20});
  DEBUG(a);
  assert(a == 2);

  a = s.countAssignments({1, 1}, {1, 1});
  DEBUG(a);
  assert(a == 0);

  a = s.countAssignments({10, 2, 10}, {100, 150, 200});
  DEBUG(a);
  assert(a == 3);

  a = s.countAssignments({10, 20}, {1, 3});
  DEBUG(a);
  assert(a == 1);

  a = s.countAssignments({20, 20, 25, 23, 24, 24, 21},
                         {20, 25, 25, 20, 25, 23, 20});
  DEBUG(a);
  assert(a == 0);

  a = s.countAssignments(
      {970, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
       800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
       800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
       800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800},
      {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
       1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
       1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
       1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
       1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000});
  DEBUG(a);
  assert(a == 318608048);
}
