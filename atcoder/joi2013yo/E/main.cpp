#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
void chmax(T &a, T b) {
  if (a < b) a = std::move(b);
}
template <class T>
void chmin(T &a, T b) {
  if (a > b) a = std::move(b);
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

template <typename T>
struct Compress {
  vector<T> vec;

  explicit Compress(vector<T> v) : vec(v) {
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
  }
  int index(T x) {
    return (int)(lower_bound(vec.begin(), vec.end(), x) - vec.begin());
  }
  T &operator[](int i) { return vec[i]; }
  int size() { return vec.size(); }
};

struct Rect {
  int x1, y1, d1, x2, y2, d2;

  bool contains(const Rect &r) const {
    return (x1 <= r.x1 and r.x2 <= x2 and y1 <= r.y1 and r.y2 <= y2 and
            d1 <= r.d1 and r.d2 <= d2);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K;
  cin >> N >> K;
  V<int> xs(2 * N), ys(2 * N), ds(2 * N);
  V<Rect> rects(N);
  REP(i, N) {
    Rect r;
    cin >> r.x1 >> r.y1 >> r.d1 >> r.x2 >> r.y2 >> r.d2;
    rects[i] = r;
    xs[2 * i] = r.x1;
    xs[2 * i + 1] = r.x2;
    ys[2 * i] = r.y1;
    ys[2 * i + 1] = r.y2;
    ds[2 * i] = r.d1;
    ds[2 * i + 1] = r.d2;
  }
  Compress<int> xc(move(xs)), yc(move(ys)), dc(move(ds));

  i64 ans = 0;
  REP(i, xc.size() - 1) {
    i64 wx = xc[i + 1] - xc[i];
    REP(j, yc.size() - 1) {
      i64 wy = yc[j + 1] - yc[j];
      REP(k, dc.size() - 1) {
        i64 wd = dc[k + 1] - dc[k];
        Rect r = {xc[i], yc[j], dc[k], xc[i + 1], yc[j + 1], dc[k + 1]};
        int count = 0;
        for (const Rect &t : rects) {
          if (t.contains(r)) ++count;
        }
        if (count >= K) {
          ans += wx * wy * wd;
        }
      }
    }
  }
  cout << ans << endl;
}
