#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define WHOLE(x) std::begin(x), std::end(x)
#define LEN(a) int((a).size())

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
  int n;
  cin >> n;
  V<i64> a(n);
  cin >> a;
  i64 c1 = 0, c2 = 0, s1 = 0, s2 = 0;  // costs and sums.
  if (a[0] == 0) {
    c1 = c2 = 1;
    s1 = 1;
    s2 = -1;
  } else if (a[0] > 0) {
    c1 = 0;
    s1 = a[0];
    c2 = a[0] + 1;
    s2 = -1;
  } else {
    c1 = -a[0] + 1;
    s1 = 1;
    c2 = 0;
    s2 = a[0];
  }

  // 1: s[0] > 0
  // 2: s[0] < 0
  auto calc = [&](i64 c, int s, bool flip) -> i64 {
    V<i64> seq(n);
    seq[0] = s;
    for (int i = 1; i < n; ++i) {
      bool neg = flip ^ (i % 2 == 1);
      if (neg) {
        assert(s > 0);
      } else {
        assert(s < 0);
      }
      if (a[i] == 0) {
        c += abs(s) + 1;
        s = (i % 2) ? -1 : 1;
      } else if (a[i] > 0 and neg) {
        c += abs(s + a[i]) + 1;
        s = -1;
      } else if (a[i] < 0 and not neg) {
        c += abs(s + a[i]) + 1;
        s = 1;
      } else if (a[i] > 0 and not neg) {
        s += a[i];
        if (s <= 0) {
          c += -s + 1;
          s = 1;
        }
      } else if (a[i] < 0 and neg) {
        s += a[i];
        if (s >= 0) {
          c += s + 1;
          s = -1;
        }
      } else {
        assert(false);
      }
      seq[i] = s;
    }
    DEBUG(seq);
    return c;
  };
  i64 ans = min(calc(c1, s1, false), calc(c2, s2, true));
  cout << ans << endl;
}
