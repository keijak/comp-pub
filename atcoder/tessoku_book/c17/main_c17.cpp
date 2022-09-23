// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

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

string ss[300010];
int si = 0;

int qf[600020];
int qb[600020];
int qfl = 300010, qfr = 300010, qbl = 300010, qbr = 300010;

auto solve() {
  int Q;
  cin >> Q;
  int n = 0;
  char qt;
  for (int i = 0; i < Q; ++i) {
    cin >> qt;
    if (qt == 'A') {
      cin >> ss[si];
      qb[qbr++] = si++;
      ++n;
      if (n & 1) {
        qf[qfr++] = qb[qbl++];
      }
    } else if (qt == 'B') {
      cin >> ss[si];
      ++n;
      if (n & 1) {
        qf[qfr++] = si;
      } else {
        qb[--qbl] = si;
      }
      ++si;
    } else if (qt == 'C') {
      ++qfl;
      --n;
      if (n & 1) {
        qf[qfr++] = qb[qbl++];
      }
    } else {
      cout << ss[qf[qfl]] << '\n';
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  solve();
  exit_();
}
