#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

std::mt19937_64 &PRNG() {
  static std::random_device seed_gen;
  static std::mt19937_64 engine(seed_gen());  // non-deterministic
  return engine;
}

void solve_tree() {
  int n, m;
  cin >> n >> m;

  auto answer = [&](const vector<int> p) -> int {
    cout << "!";
    REP(i, m) { cout << " " << p[i]; }
    cout << endl;
    int res;
    cin >> res;
    return res;
  };
  auto answer_impossible = [&]() -> int {
    cout << "! -1" << endl;
    int res;
    cin >> res;
    return res;
  };
  if (m != n - 1) {
    int res = answer_impossible();
    assert(res == 1);
    return;
  }
  bool ok = (m == 1);
  int res;
  if (ok) {
    vector<int> p(m);
    REP(i, m) p[i] = i;
    res = answer(p);
  } else {
    res = answer_impossible();
  }
  if (res < 0) {
    exit(0);
  }
}

void solve_cycle() {
  int n, m;
  cin >> n >> m;

  auto query = [&](const vector<int> &w) -> set<int> {
    cout << "?";
    REP(i, m) { cout << " " << w[i]; }
    cout << endl;
    set<int> e;
    int res;
    cin >> res;
    if (res == -1) {
      exit(0);
    }
    e.insert(res);
    REP(i, 1, n - 1) {
      int x;
      cin >> x;
      e.insert(x);
    }
    return e;
  };

  auto answer = [&](const vector<int> p) -> int {
    cout << "!";
    REP(i, m) { cout << " " << p[i]; }
    cout << endl;
    int res;
    cin >> res;
    return res;
  };
  auto answer_impossible = [&]() -> int {
    cout << "! -1" << endl;
    int res;
    cin >> res;
    return res;
  };
  if (m != n or m < 3) {
    int res = answer_impossible();
    assert(res == 1);
    return;
  }

  bool ok = true;
  i64 total_sum = 0;
  REP(i, n) total_sum += i;

  vector<int> p(m);
  REP(q, m) {
    vector<int> w(m);
    REP(i, m) { w[i] = (i == q) ? m : 1; }
    auto e = query(w);
    if (ssize(e) != n - 1) {
      ok = false;
      continue;
    }
    i64 s = accumulate(ALL(e), 0LL);
    i64 x = total_sum - s;
    if (0 <= x and x < m) {
      p[q] = x;
    } else {
      ok = false;
    }
  }
  int res;
  if (ok) {
    res = answer(p);
  } else {
    res = answer_impossible();
  }
  if (res < 0) {
    exit(0);
  }
}

void solve_general() {
  int n, m;
  cin >> n >> m;

  auto query = [&](const vector<int> &w) -> set<int> {
    cout << "?";
    REP(i, m) { cout << " " << w[i]; }
    cout << endl;
    set<int> e;
    int res;
    cin >> res;
    if (res == -1) {
      exit(0);
    }
    e.insert(res);
    REP(i, 1, n - 1) {
      int x;
      cin >> x;
      e.insert(x);
    }
    return e;
  };
  auto answer = [&](const vector<int> p) -> bool {
    cout << "!";
    REP(i, m) { cout << " " << p[i]; }
    cout << endl;
    int res;
    cin >> res;
    return res == 1;
  };
  auto answer_impossible = [&]() -> bool {
    cout << "! -1" << endl;
    int res;
    cin >> res;
    return res == 1;
  };
  std::uniform_int_distribution<int> rand(1, m);

  //   if (m == 2) {
  //     assert(answer_impossible());
  //     return;
  //   }
  if (n == m) {
    assert(answer_impossible());
    return;
  }

  REP(q, m) {
    vector<int> w(m);
    REP(i, m) w[i] = rand(PRNG());
    query(w);
  }

  if (m == 1) {
    vector<int> p(1, 0);
    assert(answer(p));
    return;
  }
  assert(answer_impossible());

  {
    // TLE
    // unsigned x = 0;
    // while (true) x ^= rand(PRNG());
    // assert(answer(vector<int>(1, x)));

    // MLE
    // size_t k = numeric_limits<size_t>::max();
    // assert(answer(vector<int>(k, 0)));
  }
}

int main() {
  int subtask;  // 1:tree, 2:cycle, 3:general
  cin >> subtask;
  int t;
  cin >> t;
  REP(i, t) {
    if (subtask == 1) {
      solve_tree();
    } else if (subtask == 2) {
      solve_cycle();
    } else {
      solve_general();
    }
  }
}
