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

template <class Monoid>
struct SegTree {
  using Func = function<Monoid(Monoid, Monoid)>;
  const Func F;
  const Monoid UNITY;
  int size;
  vector<Monoid> dat;

  SegTree(int n, const Func f, const Monoid &unity)
      : F(f), UNITY(unity), size(n), dat(2 * n, unity) {}

  // Sets i-th value (0-indexed) to x for initial setup.
  // build() must be called after set() calls.
  void set(int i, const Monoid &x) { dat[size + i] = x; }
  void build() {
    for (int k = size - 1; k > 0; --k) {
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const Monoid &x) {
    int k = size + i;
    dat[k] = x;
    while (k > 1) {
      k >>= 1;
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, open interval).
  Monoid fold(int l, int r) {
    l += size;
    r += size;
    Monoid vleft = UNITY, vright = UNITY;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = F(vleft, dat[l++]);
      if (r & 1) vright = F(dat[--r], vright);
    }
    return F(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  Monoid operator[](int i) { return dat[size + i]; }

  /* debug */
  void print() {
    for (int i = 0; i < size; ++i) {
      cout << (*this)[i];
      if (i != size - 1) cout << ",";
    }
    cout << endl;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;  // < 3e5
  cin >> n >> q;
  V<int> a(n);
  cin >> a;

  int bsize = sqrt(n + 1) + 1;
  V<int> bucket(n);
  for (int i = 0, k = 1; i < n; ++k) {
    for (; i < n and i < k * bsize; ++i) {
      bucket[i] = k - 1;
    }
  }

  V<tuple<int, int, int, int, int>> queries(q);
  REP(i, q) {
    int x, y;
    cin >> x >> y;
    int l = x;
    int r = n - y;
    queries[i] = {bucket[l], bucket[r - 1], l, r, i};
  }
  sort(WHOLE(queries));

  LazySegmentTree<int> st();

  int cl = 0, cr = 0, total = 0;
  V<int> hist(n);  // count of (i - a[i]).
  V<int> ans(q);
  REP(i, q) {
    auto [lb, rb, l, r, qi] = queries[i];
    for (; cl > l;) {
      --cl;
      int d = cl - a[cl];
      if (d >= 0) {
        st.update()
      }
      if (0 < d and)
        if (a[cl] == cl) {
          ++total;
        }
    }
    for (; cl < l; ++cl) {
      if (a[cl] == cl) {
      }
      if (--counter[colors[cl]] == 0) --ucount;
    }
    for (; cr < r; cr++) {
      int d = cr - a[cr];
      if (d < 0) continue;
      if (d <= total) {
        ++total;
      }
    }
    for (; cr > r;) {
      --cr;
      if (--counter[colors[cr]] == 0) --ucount;
    }
    ans[qi] = total;
  }

  REP(i, n) {
    // query.

    int d = i - a[i];
    if (d < 0) continue;
  }

  auto solve = [&]() -> i64 { assert(x + y < n); };
  REP(i, q) { cout << solve() << '\n'; }
}
