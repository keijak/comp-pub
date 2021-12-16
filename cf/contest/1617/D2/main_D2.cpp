#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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

int qcount;

int more_crewmates(array<int, 3> a) {
  ++qcount;
  cout << "? " << a[0] << " " << a[1] << " " << a[2] << endl;
  int res;
  cin >> res;
  if (res == -1) {
    exit(0);
  }
  return res;
}

auto solve() {
  qcount = 0;
  int n = in;
  vector<int> role(n + 1, -1);
  vector<int> group_vote(n / 3);
  int j0 = -1, j1 = -1;
  for (int i = 1; i <= n; i += 3) {
    int r = more_crewmates({i, i + 1, i + 2});
    group_vote[i / 3] = r;
    if (j0 == -1 and r == 0) j0 = i;
    if (j1 == -1 and r == 1) j1 = i;
  }
  assert(j0 != -1 and j1 != -1);
  pair<int, int> gp;
  {
    array<int, 3> q = {j0, j0 + 1, j0 + 2};
    REP(i, 3) {
      int res = 1;
      if (i < 2) {
        q[i] = j1 + i;
        res = more_crewmates(q);
      }
      if (res == 1) {
        gp = {j0 + i, j1 + i};
        role[j0 + i] = 0;
        role[j1 + i] = 1;
        break;
      }
    }
  }

  for (int i = 1; i <= n; i += 3) {
    int majority = group_vote[i / 3];
    int repl = (majority == 1) ? gp.first : gp.second;
    if (repl == i or repl == i + 1 or repl == i + 2) {
      for (int j = i; j <= i + 2; ++j) {
        if (j != repl) role[j] = majority;
      }
      continue;
    }
    array<int, 3> q = {repl, i + 1, i + 2};
    int res = more_crewmates(q);
    if (res == majority) {
      role[i + 1] = role[i + 2] = majority;
      if (i != gp.first and i != gp.second) {
        role[i] = more_crewmates({gp.first, gp.second, i});
      }
    } else {
      role[i] = majority;
      if (i + 1 != gp.first and i + 1 != gp.second) {
        role[i + 1] = more_crewmates({gp.first, gp.second, i + 1});
      }
      role[i + 2] = 1 - role[i + 1];
    }
  }
  for (int i = 1; i <= n; ++i) {
    assert(role[i] == 0 or role[i] == 1);
  }

  vector<int> imposters;
  for (int i = 1; i <= n; ++i) {
    if (role[i] == 0) imposters.push_back(i);
  }
  cout << "! " << imposters.size();
  for (auto x: imposters) {
    cout << " " << x;
  }
  cout << endl;
  if (qcount > n + 2) throw std::runtime_error("Asked more than n+2 queries.");
}

int main() {
  const int T = in;
  REP(t, T) { solve(); }
}
