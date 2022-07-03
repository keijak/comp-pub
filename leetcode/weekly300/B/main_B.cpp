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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

vector<vector<int>> spiralMatrix(int m, int n, ListNode *head) {
  static const int dj[] = {1, 0, -1, 0};
  static const int di[] = {0, 1, 0, -1};
  auto inside = [&](int x, int y) {
    return 0 <= x and x < m and 0 <= y and y < n;
  };

  auto ret = vector(m, vector(n, -1));

  int i = 0, j = 0, d = 0;
  ListNode *p = head;
  while (p != nullptr) {
    ret[i][j] = p->val;
    int ni = i + di[d];
    int nj = j + dj[d];
    int trial = 0;
    while (not inside(ni, nj) or ret[ni][nj] != -1) {
      d = (d + 1) % 4;
      ni = i + di[d];
      nj = j + dj[d];
      ++trial;
      if (trial >= 5) return ret;
    }
    i = ni;
    j = nj;
    p = p->next;
  }
  return ret;
}

int main() {
  ListNode *p = nullptr;
  vector<int> input = {3, 0, 2, 6, 8, 1, 7, 9, 4, 2, 5, 5, 0};
  reverse(ALL(input));
  for (int x: input) {
    p = new ListNode(x, p);
  }
  auto v = spiralMatrix(3, 5, p);
  DUMP(v);
  for (auto row: v) {
    DUMP(row);
  }
}
