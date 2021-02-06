#include <bits/stdc++.h>

using Int = long long;
using Float = long double;
using namespace std;

Float EPS = 1e-9;

Int solve() {
  Float X, Y, R;
  cin >> X >> Y >> R;
  if (X < 0) X *= -1;
  if (Y < 0) Y *= -1;
  Float R2 = R * R;

  Int res = 0;
  Int start = ceil(X - R);
  Int end = floor(X + R);

  for (Int i = start; i <= end; i++) {
    Float xd2 = (X - i) * (X - i);
    Float yd2 = R2 - xd2;
    Float yd = sqrt(yd2);
    Int ytop = ceil(Y + yd);
    while (ytop > Y + EPS and (ytop - Y) * (ytop - Y) > yd2 + EPS) ytop--;
    Int ybot = floor(Y - yd);
    while (ybot < Y - EPS and (Y - ybot) * (Y - ybot) > yd2 + EPS) ybot++;
    if (ytop >= ybot) {
      res += ytop - ybot + 1;
    }
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
