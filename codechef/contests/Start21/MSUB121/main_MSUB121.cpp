#include <bits/stdc++.h>
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
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;
const int L = int(1e6) + 10;

struct Node {
  int index;
  Node *p;
};

auto solve() -> vector<int> {
  int n = in;
  vector<int> a = in(n); // <= 10^6
  int Q = in;
  DUMP(Q);
  set<pair<int, int>> ranges;
  REP(i, Q) {
    int x = in, y = in;
    DUMP(x, y);
    ranges.insert(pair{x, y});
  }
  DUMP(ranges);

  vector<vector<int>> yfrom(L);
  vector<vector<int>> tobig(L);
  vector<char> isbig(L);
  for (auto[x, y]: ranges) {
    yfrom[y].push_back(x);
  }

  const int B = max(int(ceil(sqrt(n))) + 1, 100);

  REP(y, L) {
    if (ssize(yfrom[y]) >= B) {
      isbig[y] = true;
    }
  }
  for (auto[x, y]: ranges) {
    if (isbig[y]) {
      tobig[x].push_back(y);
    }
  }

  vector<int> lens(L, 0);
  vector<int> bigtemp(L, 1);
  vector<int> pre(n, -1);
  vector<int> bpre(L, -1);
  vector<int> last(L, -1);
  REP(i, n) {
    int y = a[i];
    bool len_updated = false;
    if (isbig[y]) {
      if (chmax(lens[y], bigtemp[y])) {
        pre[i] = bpre[y];
        len_updated = true;
      }
    } else {
      int maxlen = 0;
      int mi = -1;
      for (int x: yfrom[y]) {
        if (chmax(maxlen, lens[x])) {
          mi = last[x];
        }
      }
      if (chmax(lens[y], maxlen + 1)) {
        pre[i] = mi;
        len_updated = true;
      }
    }
    if (len_updated) {
      last[y] = i;
      for (int z: tobig[y]) {
        assert(isbig[z]);
        if (chmax(bigtemp[z], lens[y] + 1)) {
          bpre[z] = i;
        }
      }
    }
  }

  int maxi = *max_element(ALL(lens));
  if (maxi == 0) {
    return {};
  }
  if (maxi == 1) {
    return {0};
  }
  for (int i = n - 1; i >= 0; --i) {
    if (lens[a[i]] != maxi) continue;
    vector<int> ans;
    for (int j = last[a[i]]; j != -1; j = pre[j]) {
      ans.push_back(j);
    }
    reverse(ALL(ans));
    return ans;
  }
  assert(false);
  return {};
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans.size());
    for (auto x: ans) cout << x + 1 << ' ';
    cout << '\n';
  }
}
