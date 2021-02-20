#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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

#ifdef ENABLE_DEBUG
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

#include <bits/stdc++.h>
using namespace std;
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

struct MultiplicationTable2 {
  int n;
  vector<int> table;
  int minimalGoodSet(vector<int> table) {
    this->table = table;
    n = lround(sqrt(table.size()));
    int ans = n;
    REP(i, n) { chmin(ans, check(i)); }
    return ans;
  }

  int check(int x) {
    set<int> s;
    s.insert(x);
    vector<int> a;
    a.reserve(n);
    a.push_back(x);
    for (int i = 0; i < (int)a.size(); ++i) {
      for (int j = i; j >= 0; --j) {
        int v = table[a[i] * n + a[j]];
        if (not s.count(v)) {
          s.insert(v);
          a.push_back(v);
        }
        v = table[a[j] * n + a[i]];
        if (not s.count(v)) {
          s.insert(v);
          a.push_back(v);
        }
      }
    }
    return a.size();
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  MultiplicationTable2 s;
  int a;
  {
    a = s.minimalGoodSet({1, 1, 2, 3, 1, 0, 3, 2, 2, 0, 1, 3, 1, 2, 3, 0});
    DEBUG(a);
    assert(a == 2);
  }
  {
    a = s.minimalGoodSet({0, 1, 2, 3, 1, 2, 3, 0, 2, 3, 0, 1, 3, 0, 1, 2});
    DEBUG(a);
    assert(a == 1);
  }
  {
    a = s.minimalGoodSet({1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0, 0});
    DEBUG(a);
    assert(a == 4);
  }
  {
    a = s.minimalGoodSet({0});
    DEBUG(a);
    assert(a == 1);
  }
}
