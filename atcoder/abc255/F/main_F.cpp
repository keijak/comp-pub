// #define NDEBUG
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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));  // No input is left behind.
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
void test_case(int t, int T) {
  if (T <= 1) return;
  std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
            << std::endl;
}
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define test_case(...)
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

struct Less {
  vector<int> &pi;
  explicit Less(vector<int> &pi) : pi(pi) {}
  bool operator()(int a, int b) const { return pi[a] < pi[b]; }
};

template<class T, class Comparator = Less>
std::vector<int> construct_cartesian_tree(const std::vector<T> &a, const Comparator &cmp) {
  int n = a.size();
  std::vector<int> parent(n, -1);
  for (int i = 1; i < n; ++i) {
    int p = i - 1;  // parent of i
    int l = -1;  // left child of i
    while (p != -1 and cmp(a[i], a[p])) {
      int pp = parent[p];  // parent of parent of i
      if (l != -1) {
        parent[l] = p;
      }
      parent[p] = i;
      l = p;
      p = pp;
    }
    parent[i] = p;
  }
  return parent;
}

optional<vector<pair<int, int>>> solve() {
  const int n = in;
  vector<int> P = in(n), I = in(n);
  if (P[0] != 1) return nullopt;
  vector<int> pi(n, -1), ii(n, -1);
  REP(i, n) {
    --P[i];
    pi[P[i]] = i;
  }
  REP(i, n) {
    --I[i];
    ii[I[i]] = i;
  }
  DUMP(P);
  DUMP(pi);
  const Less cmp_p{pi};
  auto parent = construct_cartesian_tree(I, cmp_p);
  vector<pair<int, int>> ans(n, {-1, -1});

  REP(i, n) {
    int j = parent[i];
    if (j == -1) {
      if (I[i] != 0) return nullopt;
      continue;
    }
    int c = I[i];
    int p = I[j];
    if (i < j) {
      ans[p].first = c;
    } else {
      ans[p].second = c;
    }
  }

  vector<int> preord, inord;
  Rec([&](auto &dfs, int v) -> void {
    preord.push_back(v);
    if (ans[v].first != -1) {
      dfs(ans[v].first);
    }
    inord.push_back(v);
    if (ans[v].second != -1) {
      dfs(ans[v].second);
    }
  })(0);
  DUMP(preord);
  if (preord != P) return nullopt;
  if (inord != I) return nullopt;
  return ans;
}

int main() {
  init_();
  auto ans = (solve());
  if (not ans) {
    print(-1);
  } else {
    for (const auto &[L, R]: *ans) {
      print(L + 1, R + 1);
    }
  }
  exit_();
}
