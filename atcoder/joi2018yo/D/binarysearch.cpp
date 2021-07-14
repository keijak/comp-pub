#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

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
template <typename T>
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
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

#ifdef MY_DEBUG
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

const int INF = 1e8;

struct Range {
  int sum;
  int l;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  V<int> L(N);
  cin >> L;

  V<int> cum(N + 1), lmin(N + 1, INF), rmin(N + 1, INF), lmax(N + 1, 0),
      rmax(N + 1, 0);
  REP(i, N) {
    cum[i + 1] = cum[i] + L[i];
    lmin[i + 1] = min(lmin[i], L[i]);
    rmin[N - 1 - i] = min(rmin[N - i], L[N - 1 - i]);
    lmax[i + 1] = max(lmax[i], L[i]);
    rmax[N - 1 - i] = max(rmax[N - i], L[N - 1 - i]);
  }

  auto check = [&](int l, int r, int tmin, int tmax) -> bool {
    V<Range> stack(N + 1);

    auto fix_stack = [&](auto self, int cur, bool below_ok) -> bool {
      if (stack[cur].sum < tmin) return false;
      if (stack[cur].sum <= tmax) return true;
      if (cur == 0) return false;
      while (stack[cur].sum > tmax) {
        int k = stack[cur].l;
        stack[cur].sum -= L[k];
        if (stack[cur].sum < tmin and not below_ok) return false;
        stack[cur - 1].sum += L[k];
        stack[cur].l++;
      }
      return self(self, cur - 1, false);
    };

    int acc = 0;
    int from = 0;
    for (int i = l; i < r; ++i) {
      acc += L[i];
      if (acc >= tmin) {
        stack.push_back({acc, from});
        if (not fix_stack(fix_stack, stack.size() - 1, true)) {
          return false;
        }
        acc = 0;
        from = i + 1;
      }
    }
    if (acc > 0) {
      stack.push_back({acc, from});
    }
    if (not fix_stack(fix_stack, stack.size() - 1, false)) {
      return false;
    }
    return true;
  };

  int ans = lmax[N] - lmin[N];

  for (int l = 0; l < N; ++l) {
    for (int r = l + 1; r <= N; ++r) {
      if (l == 0 and r == N) continue;
      int tmax = cum[r] - cum[l];
      if (tmax < lmax[N]) continue;

      // Find the possible largest min value.
      int ok = 0, ng = tmax + 1;
      while (ng - ok > 1) {
        int mid = (ng + ok) / 2;
        if (check(0, l, mid, tmax) and check(r, N, mid, tmax)) {
          ok = mid;
        } else {
          ng = mid;
        }
      }
      if (ok == 0) continue;
      chmin(ans, tmax - ok);
      if (tmax == 19) DEBUG(tmax, l, r, ok, ans);
    }
  }
  cout << ans << endl;
}
