#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

struct VersatileInput {
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

int main() {
  int n;
  cin >> n;

  auto query = [&](vector<int> &a) -> int {
    cout << "?";
    REP(i, n) cout << " " << a[i];
    cout << endl;
    int i;
    cin >> i;
    return i - 1;
  };

  int X = n;

  vector<int> xminus(n);
  for (int i = 1; i < n; ++i) {
    vector<int> a(n);
    a[n - 1] = 1;
    REP(j, n - 1) a[j] = 1 + i;
    int res = query(a);
    if (res == -1) {
      X = i;
      break;
    }
    xminus[i] = res;
  }

  vector<int> xplus(n);
  for (int i = 1; i < n; ++i) {
    vector<int> a(n);
    a[n - 1] = n;
    REP(j, n - 1) a[j] = n - i;
    int res = query(a);
    if (res == -1) {
      int x = n + 1 - i;
      check(X == x);
      break;
    }
    xplus[i] = res;
  }

  check(X > 0);
  vector<int> ans(n);
  ans[n - 1] = X;
  for (int i = 1; i < X; ++i) {
    int j = xminus[i];
    ans[j] = X - i;
  }
  for (int i = 1; X + i <= n; ++i) {
    int j = xplus[i];
    ans[j] = X + i;
  }

  cout << "!";
  REP(i, n) cout << " " << ans[i];
  cout << endl;
}
