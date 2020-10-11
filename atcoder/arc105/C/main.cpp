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

#ifdef ENABLE_DEBUG
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
  const int L = 100'000'000;

  int n, m;
  cin >> n >> m;
  V<i64> w(n);
  cin >> w;
  V<i64> vlmax(L + 1);
  i64 lmax = 0;
  i64 vmin = 1e9;
  REP(i, m) {
    i64 l, v;
    cin >> l >> v;
    chmax(vlmax[v], l);
    chmax(lmax, l);
    chmin(vmin, v);
  }
  if (vmin < *max_element(ALL(w))) {
    cout << -1 << endl;
    exit(0);
  }
  REP(i, L) { vlmax[i + 1] = max(vlmax[i], vlmax[i + 1]); }

  auto get_len = [&](i64 w) {
    if (w > L) return lmax;
    return vlmax[w - 1];
  };

  V<int> camels(n);
  REP(i, n) { camels[i] = i; }
  auto dp = vector(n, vector(n, optional<pair<i64, i64>>()));
  auto mindist = [&](auto self, int l, int r) -> pair<i64, i64> {
    assert(l < r);
    if (r - l == 1) {
      i64 weight = w[camels[l]] + w[camels[r]];
      i64 len = get_len(weight);
      return {weight, len};
    }
    if (dp[l][r].has_value()) return dp[l][r].value();
    i64 weight = -1, len = 0;
    for (int k = l + 1; k < r; ++k) {
      auto [w1, len1] = self(self, l, k);
      auto [w2, len2] = self(self, k, r);
      i64 ww = w1 + w2 - w[camels[k]];
      if (weight == -1) {
        weight = ww;
        len = get_len(weight);
      }
      chmax(len, len1 + len2);
      assert(weight == ww);
    }
    auto res = make_pair(weight, len);
    dp[l][r] = res;
    return res;
  };

  i64 ans = (n - 1) * lmax;
  do {
    dp = vector(n, vector(n, optional<pair<i64, i64>>()));
    auto [weight, len] = mindist(mindist, 0, n - 1);
    chmin(ans, len);
  } while (next_permutation(ALL(camels)));

  cout << ans << endl;
}
