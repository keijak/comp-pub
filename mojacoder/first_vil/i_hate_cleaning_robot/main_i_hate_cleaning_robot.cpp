#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

using namespace std;

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::cpp_int;
namespace multip = boost::multiprecision;

pair<Int, Int> calc(const vector<int> &d1, const vector<int> &d2) {
  const int kOffset = 2000010 / 2;
  BigInt bits = BigInt(1) << kOffset;
  for (auto d: d1) bits |= bits << d;
  for (auto d: d2) bits |= bits >> d;
  Int high = accumulate(ALL(d1), 0LL);
  Int low = accumulate(ALL(d2), 0LL);
  Int popcount = 0;
  const unsigned l = multip::lsb(bits), r = multip::msb(bits);
  for (unsigned i = l; i <= r; ++i) {
    if (multip::bit_test(bits, i)) ++popcount;
  }
  return {popcount, high + low + 1};
}

auto solve() {
  int n;
  cin >> n;
  vector<vector<int>> deltas(128);
  REP(i, n) {
    char c;
    int d;
    cin >> c >> d;
    deltas[c].push_back(d);
  }
  auto[xcnt, xwidth] = calc(deltas['R'], deltas['L']);
  auto[ycnt, ywidth] = calc(deltas['U'], deltas['D']);
  return xcnt * ywidth + ycnt * xwidth - xcnt * ycnt;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << endl;
}
