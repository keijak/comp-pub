#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(l) (int)((l).size())

template <class T>
inline bool chmax(T &l, T r) {
  return l < r and ((l = std::move(r)), true);
}
template <class T>
inline bool chmin(T &l, T r) {
  return l > r and ((l = std::move(r)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &l) {
  for (auto &x : l) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &l, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto r = std::begin(l), e = std::end(l);
  for (auto it = std::begin(l); it != e; ++it) {
    if (it != r) *os << sep;
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
std::ostream &operator<<(std::ostream &os, const T &l) {
  return pprint(l, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &l) {
  return os << "(" << l.first << ", " << l.second << ")";
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

  int N;
  cin >> N;
  auto D = vector(N, vector(N, 0));
  REP(i, N) REP(j, N) { cin >> D[i][j]; }
  int Q;
  cin >> Q;

  auto DS = vector(N + 1, vector(N + 1, 0));
  REP(i, N) REP(j, N) DS[i + 1][j + 1] = D[i][j];
  REP(i, N) REP(j, N) { DS[i + 1][j + 1] += DS[i + 1][j]; }
  REP(i, N) REP(j, N) { DS[i + 1][j + 1] += DS[i][j + 1]; }

  const int N2 = N * N;
  V<int> cummax(N2 + 5, -1e8);
  for (int si = 0; si < N; ++si) {
    for (int sj = 0; sj < N; ++sj) {
      for (int i = si + 1; i <= N; ++i) {
        int di = i - si;
        for (int j = sj + 1; j <= N; ++j) {
          int num = di * (j - sj);
          int taste = DS[i][j] - DS[si][j] - DS[i][sj] + DS[si][sj];
          chmax(cummax[num], taste);
        }
      }
    }
  }
  for (int i = 1; i <= N2; ++i) {
    chmax(cummax[i], cummax[i - 1]);
  }

  REP(q, Q) {
    int p;
    cin >> p;
    cout << cummax[p] << '\n';
  }
}
