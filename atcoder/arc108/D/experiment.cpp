#include <bits/stdc++.h>

#include <atcoder/modint>

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

using namespace std;

using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  char caa, cab, cba, cbb;
  // cin >> n >> caa >> cab >> cba >> cbb;
  caa = cab = cba = cbb = 'B';

  map<array<char, 2>, char> chmap;
  chmap[{'A', 'A'}] = caa;
  chmap[{'A', 'B'}] = cab;
  chmap[{'B', 'A'}] = cba;
  chmap[{'B', 'B'}] = cbb;

  set<list<char>> st;

  auto f = [&](auto self, list<char> &s) -> void {
    int m = ssize(s);
    if (m == n) {
      st.insert(s);
      return;
    }
    auto it = s.begin();
    ++it;
    for (; it != s.end(); ++it) {
      char c2 = *it;
      char c1 = *std::prev(it);
      char cx = chmap[{c1, c2}];
      auto it2 = s.insert(it, cx);
      self(self, s);
      s.erase(it2);
    }
  };

  auto solve = [&]() -> int {
    list<char> s = {'A', 'B'};
    f(f, s);
    // DEBUG(st);
    return ssize(st);
  };

  for (char caa = 'A'; caa != 'C'; ++caa)
    for (char cab = 'A'; cab != 'C'; ++cab)
      for (char cba = 'A'; cba != 'C'; ++cba)
        for (char cbb = 'A'; cbb != 'C'; ++cbb) {
          chmap[{'A', 'A'}] = caa;
          chmap[{'A', 'B'}] = cab;
          chmap[{'B', 'A'}] = cba;
          chmap[{'B', 'B'}] = cbb;
          vector<int> res;
          REP(i, 2, 10) {
            n = i;
            st.clear();
            auto r = solve();
            res.push_back(r);
          }
          string x = {caa, cab, cba, cbb};
          DEBUG(x, res);
        }
}
