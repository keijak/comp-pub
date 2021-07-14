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

void solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;
  int zero = 0;
  int nega = 0;
  REP(i, n) {
    if (a[i] == 0) {
      ++zero;
    } else if (a[i] < 0) {
      ++nega;
    }
  }
  DEBUG(zero, nega);
  vector<int> idx(n);
  REP(i, n) idx[i] = i;
  sort(ALL(idx), [&](int i, int j) { return abs(a[i]) < abs(a[j]); });
  DEBUG(idx);
  REP(i, n) { DEBUG(i, a[idx[i]]); }

  int sneg_idx = -1;
  REP(i, n) {
    if (a[idx[i]] < 0) {
      sneg_idx = idx[i];
      break;
    }
  }

  if (zero == 0) {
    if (nega & 1) {
      assert(sneg_idx >= 0);
      cout << "2 " << sneg_idx + 1 << "\n";
      int p = n - 1;
      if (idx[p] == sneg_idx) --p;
      for (int i = p - 1; i >= 0; --i) {
        if (idx[i] == sneg_idx) continue;
        cout << "1 " << idx[p] + 1 << " " << idx[i] + 1 << "\n";
        p = i;
      }
    } else {
      int p = n - 1;
      for (int i = p - 1; i >= 0; --i) {
        cout << "1 " << idx[p] + 1 << " " << idx[i] + 1 << "\n";
        p = i;
      }
    }
    return;
  }

  int zero_idx = -1;  // last zero
  REP(i, n) {
    if (a[idx[i]] != 0) break;
    zero_idx = idx[i];
  }
  assert(zero_idx != -1);

  int count = 0;
  if (nega & 1) {
    assert(sneg_idx >= 0);
    cout << "1 " << sneg_idx + 1 << " " << zero_idx + 1 << "\n";
    ++count;
  }

  REP(i, n) {
    if (idx[i] == zero_idx) break;
    cout << "1 " << idx[i] + 1 << " " << zero_idx + 1 << "\n";
    ++count;
  }
  if (count == n - 1) return;

  cout << "2 " << zero_idx + 1 << "\n";
  ++count;

  int p = n - 1;
  if (idx[p] == sneg_idx) --p;
  for (int i = p - 1; i >= 0; --i) {
    if (count == n - 1) break;
    if (a[idx[i]] == 0) break;
    if ((nega & 1) and idx[i] == sneg_idx) continue;
    cout << "1 " << idx[p] + 1 << " " << idx[i] + 1 << "\n";
    ++count;
    p = i;
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
