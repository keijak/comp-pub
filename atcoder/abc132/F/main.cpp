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

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

i64 solve() {
  i64 limit, k;
  cin >> limit >> k;
  i64 L = sqrt(limit) + 1e-9;
  vector<int> counterpart(L + 2), bucket_size(L + 2);
  L = 0;
  for (int x = 1;; ++x) {
    if (limit / x < x) break;
    L = x;
    counterpart[x] = limit / x;
    bucket_size[x] = limit / x - limit / (x + 1);
  }
  const int LC = limit / L;
  DEBUG(L, LC);
  vector dpl(L + 1, Mint(0));
  vector dph(L + 1, Mint(0));
  dpl[1] = 1;

  vector<Mint> cuml(L + 1, Mint(0)), cumh(L + 1, Mint(0));
  REP(i, k) {
    cuml.assign(L + 1, Mint(0));
    cumh.assign(L + 1, Mint(0));

    for (int j = 1; j <= L; ++j) {
      cuml[j] = cuml[j - 1] + dpl[j];
    }
    if (LC == L) {
      cumh[L] = cuml[L];
    } else {
      cumh[L] = cuml[L] + dph[L] * bucket_size[L];
    }
    for (int j = L - 1; j >= 1; --j) {
      cumh[j] = cumh[j + 1] + dph[j] * bucket_size[j];
    }
    REP(j, 1, L + 1) { dph[j] = cuml[j]; }
    REP(j, 1, L + 1) { dpl[j] = cumh[j]; }

    // for (int x = 1; x <= L; ++x) {
    //   DEBUG(x, cuml[x], cumh[x]);
    // }
    // for (int x = 1; x <= L; ++x) {
    //   DEBUG(x, dpl[x]);
    // }
    // for (int x = L + 1; x <= limit; ++x) {
    //   int y = limit / x;
    //   DEBUG(x, dph[y]);
    // }
  }
  Mint ans = 0;
  REP(i, 1, L + 1) ans += dpl[i];
  REP(i, 1, LC) { ans += dph[i] * bucket_size[i]; }
  return ans.val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
