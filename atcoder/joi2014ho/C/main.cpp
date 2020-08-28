#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  V<i64> A(N);
  REP(i, N) cin >> A[i];
  i64 ans = 0;

  V<i64> cum(N + 1);
  REP(i, N) { cum[i + 1] = cum[i] + A[i]; }
  i64 total = cum[N];

  auto range_sum = [&](int l, int r) -> i64 {
    if (l <= r) {
      return cum[r] - cum[l];
    } else {
      return (cum[N] - cum[l]) + cum[r];
    }
  };

  auto check = [&](int l, int x) -> bool {
    int r = (l + x) % N;
    i64 my = range_sum(l, r);

    i64 offset = cum[r];
    if (total >= my + offset) {
      auto it = lower_bound(cum.begin() + r, cum.end(), my + offset);
      assert(it != cum.end());
      i64 a = (*it) - offset;
      assert(a >= my);
      return (total - my - a >= my);
    }

    if (cum[l] >= my) {
      auto it = upper_bound(cum.begin(), cum.end(), cum[l] - my);
      assert(it != cum.begin());
      --it;
      i64 a = cum[l] - *it;
      assert(a >= my);
      return (total - my - a >= my);
    }

    return false;
  };

  REP(i, N) {
    int ok = 0, ng = N;
    while (ng - ok > 1) {
      int mid = (ng + ok) / 2;
      (check(i, mid) ? ok : ng) = mid;
    }
    ans = max(ans, range_sum(i, (i + ok) % N));
  }
  cout << ans << endl;
}
