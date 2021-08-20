#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() -> vector<i64> {
  INPUT(i64, n, m);
  vector<i64> x(n);
  cin >> x;
  vector<char> dir(n);
  cin >> dir;
  DUMP(dir);
  vector<i64> ans(n, -1);

  vector<pair<i64, int>> sx(n);
  REP(i, n) {
    sx[i] = {x[i], i};
  }
  sort(ALL(sx));

  map<i64, int> le, lo, re, ro;
  REP(i, n) {
    if (dir[i] == 'L') {
      if (x[i] % 2 == 0) {
        le[x[i]] = i;
      } else {
        lo[x[i]] = i;
      }
    } else {
      if (x[i] % 2 == 0) {
        re[x[i]] = i;
      } else {
        ro[x[i]] = i;
      }
    }
  }

  auto f = [&](map<i64, int> &rm, map<i64, int> &lm) {
    vector<pair<i64, int>> rev(ALL(rm));
    for (int p = ssize(rev) - 1; p >= 0; --p) {
      auto[pos, i] = rev[p];
      auto it = lm.upper_bound(x[i]);
      if (it != lm.end()) {
        int j = it->second;
        i64 t = (it->first - x[i]) / 2;
        ans[i] = ans[j] = t;
        lm.erase(it);
        rm.erase(x[i]);
      }
    }
  };

  f(re, le);
  f(ro, lo);

  for (auto *lm : {&le, &lo}) {
    deque<int> q;
    for (auto[xx, i] : *lm) { q.push_back(i); }
    while (not q.empty()) {
      int i = q.front();
      q.pop_front();
      if (q.empty()) break;
      int j = q.front();
      q.pop_front();
      i64 t = x[i];
      t += (x[j] - x[i]) / 2;
      assert(ans[i] == -1);
      assert(ans[j] == -1);
      ans[i] = ans[j] = t;
      lm->erase(x[i]);
      lm->erase(x[j]);
    }
  }
  for (map<i64, int> *rm : {&re, &ro}) {
    vector<int> rev;
    for (auto[xx, i] : *rm) { rev.push_back(i); }
    while (not rev.empty()) {
      int i = rev.back();
      rev.pop_back();
      if (rev.empty()) break;
      int j = rev.back();
      rev.pop_back();
      i64 t = m - x[i];
      t += (x[i] - x[j]) / 2;
      assert(ans[i] == -1);
      assert(ans[j] == -1);
      ans[i] = ans[j] = t;
      rm->erase(x[i]);
      rm->erase(x[j]);
    }
  }

  auto g = [&](map<i64, int> &rm, map<i64, int> &lm) {
    if (lm.size() and rm.size()) {
      DUMP(lm, rm);
      assert(lm.size() == 1u and rm.size() == 1u);
      auto[lx, li] = *lm.begin();
      auto[rx, ri] = *rm.begin();
      i64 t = max(m - rx, lx);
      i64 d = max(m - rx, lx) - min(m - rx, lx);
      t += (m - d) / 2;
      ans[li] = ans[ri] = t;
      lm.erase(lm.begin());
      rm.erase(rm.begin());
    }
  };

  g(re, le);
  g(ro, lo);

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  cin >> t;
  REP(test_case, t) {
    auto ans = solve();
    print_seq(ans);
  }
}
