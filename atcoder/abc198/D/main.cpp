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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

optional<array<int, 26>> solve(const array<string, 3> &s) {
  {
    int x = int(ssize(s[2])) - max(ssize(s[0]), ssize(s[1]));
    if (x > 1 or x < 0) return nullopt;
  }
  set<char> used;
  REP(i, 3) {
    for (char c : s[i]) {
      used.insert(c);
    }
  }
  const int m = ssize(used);
  if (m > 10) return nullopt;
  array<int, 26> ch2num;
  fill(ALL(ch2num), -1);
  array<int, 10> num2ch;
  fill(ALL(num2ch), -1);

  auto cand = [&](int ch) -> pair<vector<int>, bool> {
    vector<int> r;
    if (ch < 0) {
      r.push_back(0);
      return {move(r), false};
    }
    if (ch2num[ch] != -1) {
      r.push_back(ch2num[ch]);
      return {move(r), false};
    }
    for (int i = 0; i < 10; ++i) {
      if (num2ch[i] == -1) {
        r.push_back(i);
      }
    }
    return {move(r), true};
  };

  auto check = [&](auto &rec, int i, bool carry) -> bool {
    const int xc = (i < ssize(s[0])) ? (s[0][ssize(s[0]) - 1 - i] - 'a') : -1;
    const int yc = (i < ssize(s[1])) ? (s[1][ssize(s[1]) - 1 - i] - 'a') : -1;
    const int zc = (i < ssize(s[2])) ? (s[2][ssize(s[2]) - 1 - i] - 'a') : -1;
    if (i == ssize(s[2])) {
      return xc == -1 and yc == -1 and zc == -1 and !carry;
    }

    auto [xs, xu] = cand(xc);
    for (auto x : xs) {
      if (x == 0 and i == ssize(s[0]) - 1) continue;
      if (xu) {
        ch2num[xc] = x;
        num2ch[x] = xc;
      }
      auto [ys, yu] = cand(yc);
      for (auto y : ys) {
        if (y == 0 and i == ssize(s[1]) - 1) continue;
        if (yu) {
          ch2num[yc] = y;
          num2ch[y] = yc;
        }
        auto [zs, zu] = cand(zc);
        for (auto z : zs) {
          if (z == 0 and i == ssize(s[2]) - 1) continue;
          int a = x + y + int(carry);
          if (a % 10 == z) {
            if (zu) {
              ch2num[zc] = z;
              num2ch[z] = zc;
            }
            if (rec(rec, i + 1, (a >= 10))) return true;
            if (zu) ch2num[zc] = num2ch[z] = -1;
          }
        }
        if (yu) ch2num[yc] = num2ch[y] = -1;
      }
      if (xu) ch2num[xc] = num2ch[x] = -1;
    }
    return false;
  };

  if (check(check, 0, false)) return ch2num;
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  array<string, 3> s;
  REP(i, 3) { cin >> s[i]; }
  auto res = solve(s);
  if (!res) {
    cout << "UNSOLVABLE\n";
  } else {
    auto ch2num = res.value();
    REP(i, 3) {
      REP(j, ssize(s[i])) {
        int d = ch2num[s[i][j] - 'a'];
        putchar(d + '0');
      }
      putchar('\n');
    }
  }
}
