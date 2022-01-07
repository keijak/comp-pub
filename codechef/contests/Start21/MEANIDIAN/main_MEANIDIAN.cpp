#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template<class Key, class T>
using rb_tree_map = __gnu_pbds::tree<Key, T, std::less<Key>, __gnu_pbds::rb_tree_tag,
                                     __gnu_pbds::tree_order_statistics_node_update>;

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

auto solve() -> Int {
  int n = in;
  vector<Int> a = in(n);
  sort(ALL(a));
  int mid = (n - 1) / 2;
  Int med = a[mid];
  Int sum = accumulate(ALL(a), 0LL);
  DUMP(mid, med, sum, a);
  if (n * med == sum) return 0LL;

  Int ans = 0;
  if (n * med > sum) {
    Int delta = n * med - sum;
    for (int i = 0; i < mid; ++i) {
      Int x = min(med - a[i], delta);
      a[i] += x;
      delta -= x;
      sum += x;
      ans += x;
      if (delta == 0) break;
    }
    assert(delta == 0);
    sort(ALL(a));
    DUMP(a);
    assert(a[mid] == med);
    if (n * med == sum) return ans;
  }

  rb_tree_map<pair<Int, int>, int> sa;
  REP(i, mid) {
    sa[pair{a[i], i}] = 1;
  }
  {
    assert(a[mid] == med);
    int i = mid + 1;
    Int cur = med;
    int cnt = 1;
    for (; i < n; ++i) {
      if (a[i] == cur) {
        ++cnt;
      } else {
        sa[pair{cur, i}] = cnt;
        cur = a[i];
        cnt = 1;
      }
    }
    assert(cnt > 0);
    sa[pair{cur, n}] = cnt;
    DUMP(sa);
  }
  sa[pair{kBigVal<Int>, 0}] = 0;

  while (true) {
    auto it = sa.find_by_order(mid);
    assert (it != sa.end());
    med = it->first.first;
    int cnt = it->second;
    DUMP(sum, med, cnt, sa, ans);

    if (n * med >= sum) {
      if (Int r = sum % n; r != 0) {
        ans += n - r;
      }
      break;
    }

    auto nit = it;
    ++nit;
    assert(nit != sa.end());
    assert(nit->first.first > med);

    Int c = ceil_div<Int>(sum - n * med, n - 1);
    chmin(c, nit->first.first - med);
    sum += c * cnt;
    ans += c * cnt;
    auto newval = pair{med + c, it->second};
    int newcnt = cnt;
    sa.erase(it);
    while (nit->first.first == med + c) {
      newcnt += nit->second;
      nit = sa.erase(nit);
    }
    sa[newval] = newcnt;
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
