#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N, K;
  cin >> N >> K;
  V<i64> A(N);
  cin >> A;

  auto dpl = vector(N + 1, vector(K + 1, false));
  auto dpr = vector(N + 1, vector(K + 1, false));
  dpl[0][0] = dpr[N][0] = true;

  for (int i = 1; i <= N; ++i) {
    int xl = A[i - 1];
    int xr = A[N - i];
    dpl[i] = dpl[i - 1];
    for (int j = 0; xl + j <= K; ++j) {
      if (dpl[i - 1][j]) dpl[i][j + xl] = true;
    }
    int ri = N - i;
    dpr[ri] = dpr[ri + 1];
    for (int j = 0; xr + j <= K; ++j) {
      if (dpr[ri + 1][j]) dpr[ri][j + xr] = true;
    }
  }

  int ans = 0;
  REP(i, N) {
    bool necessary = false;
    int r = K - 1;
    for (int l = 0; l < K; ++l) {
      if (not dpl[i][l]) continue;
      while (l + r >= K or (r >= 0 and not dpr[i + 1][r])) {
        --r;
      }
      if (r < 0) break;
      if (l + r + A[i] >= K) {
        necessary = true;
        break;
      }
    }
    if (not necessary) {
      ++ans;
    }
  }
  cout << ans << endl;
}
