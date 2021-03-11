#include <bits/stdc++.h>
using namespace std;

struct PaintTheRoom {
  string canPaintEvenly(int R, int C, int K) {
    bool ok = (K == 1) or (R * C) % 2 == 0;
    return ok ? "Paint" : "Cannot paint";
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  PaintTheRoom solver;
  assert(solver.canPaintEvenly(1, 1, 1) == "Paint");
  assert(solver.canPaintEvenly(1, 1, 2) == "Cannot paint");
  assert(solver.canPaintEvenly(1, 2, 2) == "Paint");
  assert(solver.canPaintEvenly(2, 2, 3) == "Paint");

  cout << "ok" << endl;
}
