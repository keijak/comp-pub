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

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<pair<char, unsigned>> prog(n);
  REP(i, n) {
    char op;
    unsigned x;
    cin >> op >> x;
    prog[i] = {op, x};
  }
  auto run = [&](const vector<pair<char, unsigned>> &p,
                 unsigned x) -> unsigned {
    REP(i, n) {
      auto [op, c] = p[i];
      if (op == '&') {
        x &= c;
      } else if (op == '|') {
        x |= c;
      } else {
        x ^= c;
      }
    }
    return x;
  };

  unsigned r0 = run(prog, 0);
  unsigned r1 = run(prog, 1023);
  unsigned y_and = 1023, y_or = 0, y_xor = 0;
  REP(i, 10) {
    if ((r0 & (1 << i)) and (r1 & (1 << i))) {
      y_or |= (1 << i);
    } else if (r0 & (1 << i)) {
      y_xor |= (1 << i);
    } else if (r1 & (1 << i)) {
      // nop
    } else {
      y_and &= ~(1 << i);
    }
  }
  vector<pair<char, unsigned>> ans;
  ans.emplace_back('&', y_and);
  ans.emplace_back('|', y_or);
  ans.emplace_back('^', y_xor);

  // REP(x, 1024) { assert(run(ans, x) == run(prog, x)); }

  cout << "3\n";
  for (auto [op, y] : ans) {
    cout << op << ' ' << y << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
