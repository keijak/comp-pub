#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, L, T;
  cin >> N >> L >> T;
  vector<i64> X(N);
  vector<short> W(N);
  vector<pair<i64, int>> rx, lx;
  rx.reserve(N);
  lx.reserve(N);
  int target = -1, lp = -1;
  REP(i, N) {
    cin >> X[i] >> W[i];
    if (W[i] == 1) {
      rx.emplace_back(X[i], i);
      if (target == -1) target = i;
    } else {
      assert(W[i] == 2);
      if (target != -1 && lp == -1) {
        lp = lx.size();
      }
      lx.emplace_back(X[i], i);
    }
  }
  if (lp == -1) lp = 0;

  auto modL = [&](i64 x) -> i64 {
    if (x >= 0) return x % L;
    i64 k = (-x + L - 1) / L;
    return (x + k * L) % L;
  };

  DEBUG(target);
  vector<int> pos(N);
  REP(i, N) { pos[i] = modL(X[i] + T * (W[i] == 1 ? 1 : -1)); }

  if (rx.empty() || lx.empty()) {
    for (auto x : pos) {
      cout << x << '\n';
    }
    return 0;
  }

  auto track_target = [&]() -> pair<i64, int> {
    i64 t = 0;
    i64 x = X[target];
    const int rsize = rx.size();
    const int lsize = lx.size();
    int rp = (target - 1 + rsize) % rsize;
    DEBUG(rp, lp);
    while (true) {
      auto [x2, i2] = lx[lp];
      x2 = modL(x2 - t);
      lp = (lp + 1) % lsize;
      i64 d = (x2 != x) ? modL(L + x2 - x) : L;
      DEBUG(t, x, x2, lp, d);
      if (t + d / 2 >= T) {
        return {modL(x + T - t), 1};
      }
      t += (d + 1) / 2;
      x = modL(x2 - (d + 1) / 2);

      auto [x3, i3] = rx[rp];
      x3 = modL(x3 + t);
      rp = (rp - 1 + rsize) % rsize;
      d = (x3 != x) ? modL(L + x - x3) : L;
      DEBUG(t, x, x3, rp, d);
      if (t + d / 2 >= T) {
        return {modL(x - (T - t)), 2};
      }
      t += (d + 1) / 2;
      x = modL(x3 + (d + 1) / 2);
    }
    return {-1, -1};
  };
  auto [target_pos, target_dir] = track_target();
  DEBUG(target_pos, target_dir);

  sort(pos.begin(), pos.end());
  REP(i, N) { DEBUG(i, pos[i]); }
  i64 offset = -1;
  REP(i, N) {
    if ((target_dir == 1 && pos[i] == target_pos &&
         (i == 0 || pos[i - 1] != target_pos)) ||
        (target_dir == 2 && pos[i] == target_pos &&
         (i == N - 1 || pos[i + 1] != target_pos))) {
      offset = (i - target + N) % N;
      break;
    }
  }
  DEBUG(offset);
  assert(offset >= 0);
  REP(i, N) {
    int j = (offset + i) % N;
    cout << pos[j] << '\n';
  }
}
