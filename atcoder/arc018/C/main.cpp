#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), N_##i = (b); i < N_##i; ++i)
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

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector room(n, vector(m, 0));
  vector<int> grades(n * m, 0);
  {
    i64 x0, a, p;
    cin >> x0 >> a >> p;

    if (a % p == 0) {
      i64 x1 = (x0 + a) % p;
      if (x0 <= x1) return 0;
      return (n - 1) * 2;
    }

    i64 x = x0;
    REP(i, n) REP(j, m) {
      room[i][j] = grades[i * m + j] = x;
      x = (x + a) % p;
    }
    sort(ALL(grades));
    DEBUG(grades);
  }
  REP(i, n - 1) { assert(grades[i] != grades[i + 1]); }

  //   cerr << "===" << endl;
  //   REP(i, n) { DEBUG(room[i]); }

  vector counts(n, vector(m, 0));
  i64 cost = 0;
  REP(i, n) REP(j, m) {
    int x = room[i][j];
    auto [lit, rit] = equal_range(ALL(grades), x);
    int l = lit - grades.begin();
    int r = rit - grades.begin();
    --r;
    DEBUG(l, r, l / m, r / m);
    if (l / m <= i and i <= r / m) {
      DEBUG(i, j, "STAY");
      ++counts[i][j];
    } else if (i < l / m) {
      DEBUG(i, j, l / m);
      ++counts[l / m][j];
      cost += l / m - i;
    } else {
      DEBUG(i, j, r / m);
      ++counts[r / m][j];
      cost += i - r / m;
    }
  }
  //   cerr << "===" << endl;
  //   REP(i, n) { DEBUG(counts[i]); }

  REP(i, n) {
    i64 rcost = 0;
    i64 stk = 0;
    REP(j, m) {
      i64 c = counts[i][j];
      rcost += abs(stk);
      if (c > 1) {
        stk += c - 1;
      } else if (c == 0) {
        --stk;
      }
      DEBUG(i, j, stk, rcost);
    }
    assert(stk == 0);
    cost += rcost;
  }

  return cost;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
