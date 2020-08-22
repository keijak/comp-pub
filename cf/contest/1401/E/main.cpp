#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>
template <class Key>
using rb_tree_set =
    __gnu_pbds::tree<Key, __gnu_pbds::null_type, std::less<Key>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

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
  return pprint(a, ",", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ",", "", &(os << "{")) << "}";
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

const int HORIZ = 2, BOTTOM = 3, TOP = 1;

struct Seg {
  int y;
  int segtype;
  int lx, rx;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  const int LIM = 1'000'000;

  V<Seg> segs;
  segs.reserve(N + M);
  // Horizontal segments.
  REP(i, N) {
    int y, lx, rx;
    cin >> y >> lx >> rx;
    segs.push_back({y, HORIZ, lx, rx});
  }
  segs.push_back({LIM, HORIZ, 0, LIM});

  rb_tree_set<int> xs;
  xs.insert(0);
  xs.insert(LIM);

  REP(i, M) {
    int x, by, ty;
    cin >> x >> by >> ty;
    if (by == 0) {
      segs.push_back({ty, BOTTOM, x, -1});
      xs.insert(x);
    } else {
      assert(ty == LIM);
      segs.push_back({by, TOP, x, -1});
    }
  }
  sort(segs.begin(), segs.end(), [](const Seg &s1, const Seg &s2) -> bool {
    if (s1.y != s2.y) return s1.y < s2.y;
    if (s1.segtype != s2.segtype) return s1.segtype < s2.segtype;
    if (s1.lx != s2.lx) return s1.lx < s2.lx;
    return s1.rx < s2.rx;
  });

  i64 ans = 0;
  set<int> bufx;
  for (const Seg &seg : segs) {
    if (seg.segtype == HORIZ) {
      if (seg.lx == 0) {
        auto it = xs.upper_bound(seg.rx);
        assert(it != xs.begin());
        --it;
        int c = xs.order_of_key(*it);
        DEBUG(seg.y, "LEFT", seg.lx, seg.rx, c, ans);
        ans += c;

        auto bend = bufx.upper_bound(seg.rx);
        for (auto bit = bufx.begin(); bit != bend; ++bit) {
          xs.insert(*bit);
        }
        bufx.erase(bufx.begin(), bend);
      } else {
        assert(seg.rx == LIM);
        auto it = xs.lower_bound(seg.lx);
        assert(it != xs.end());

        int c = int(xs.size()) - xs.order_of_key(*it) - 1;
        ans += c;
        DEBUG(seg.y, "RIGHT", seg.lx, seg.rx, c, ans);

        auto bstart = bufx.lower_bound(seg.lx);
        for (auto bit = bstart; bit != bufx.end(); ++bit) {
          xs.insert(*bit);
        }
        bufx.erase(bstart, bufx.end());
      }

    } else if (seg.segtype == BOTTOM) {
      xs.erase(seg.lx);
      DEBUG(seg.y, "BOTTOM", seg.lx, xs.size());
    } else {
      assert(seg.segtype == TOP);
      bufx.insert(seg.lx);
      DEBUG(seg.y, "TOP", seg.lx, xs.size());
    }
  }

  cout << ans << endl;
}
