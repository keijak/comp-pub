#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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

int main() {
  int N;
  scanf("%d", &N);
  vector<tuple<int, int>> times(N);
  int DAYM = 24 * 60;
  REP(i, N) {
    int h1, m1, h2, m2;
    scanf("%d:%d %d:%d", &h1, &m1, &h2, &m2);
    int t1 = (h1 * 60 + m1) % DAYM;
    int t2 = (h2 * 60 + m2) % DAYM;
    times[i] = {t2, t1};
  }
  sort(times.begin(), times.end());
  vector<vector<bool>> table(N, vector<bool>(24 * 60, true));

  auto available = [&](const vector<bool> &t, int start, int end) -> bool {
    if (start < end) {
      for (int i = start; i < end; ++i) {
        if (!t[i]) return false;
      }
    } else {
      for (int i = start; i < DAYM; ++i) {
        if (!t[i]) return false;
      }
      for (int i = 0; i < end; ++i) {
        if (!t[i]) return false;
      }
    }
    return true;
  };

  auto fill_table = [&](vector<bool> &t, int start, int end) {
    if (start < end) {
      for (int i = start; i < end; ++i) {
        t[i] = false;
      }
    } else {
      for (int i = start; i < DAYM; ++i) {
        t[i] = false;
      }
      for (int i = 0; i < end; ++i) {
        t[i] = false;
      }
    }
  };

  int ans = 0;
  REP(i, N) {
    auto [t2, t1] = times[i];
    for (int j = ans - 1; j >= 0; --j) {
      if (available(table[j], t1, t2)) {
        DEBUG(i, j, t1, t2);
        fill_table(table[j], t1, t2);
        goto cont;
      }
    }
    fill_table(table[ans++], t1, t2);
  cont:
    continue;
  }

  printf("%d\n", ans);
}
