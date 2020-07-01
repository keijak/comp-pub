#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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
  vector<i64> rx, lx;
  rx.reserve(N);
  lx.reserve(N);
  int target = -1, target_l = -1;
  REP(i, N) {
    cin >> X[i] >> W[i];
    if (W[i] == 1) {
      rx.push_back(X[i]);
      if (target == -1) {
        target = i;
        target_l = lx.size();
      }
    } else {
      assert(W[i] == 2);
      lx.push_back(X[i]);
    }
  }

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

  i64 x = X[target];
  assert(W[target] == 1);
  const int lsize = lx.size();
  target_l %= lsize;
  i64 t = 0;
  i64 x1 = -1;
  for (;;) {
    x1 = modL(lx[target_l] - t);
    DEBUG(t, target, x, x1);
    target_l = (target_l + 1) % lsize;
    i64 d = modL(L + x1 - x);
    if (2 * (T - t) <= d) {
      x = modL(x + T - t);
      x1 = modL(L + x1 - (T - t));
      break;
    }
    t += d / 2 + 1;
    x = modL(x + d / 2 + 1);
    target = (target + 1) % N;
  }

  DEBUG(x, x1);
  sort(pos.begin(), pos.end());
  REP(i, N) { DEBUG(i, pos[i]); }

  i64 offset = -1;
  REP(i, N) {
    if (pos[i] == x) {
      offset = i;
      break;
    }
  }
  DEBUG(offset);
  assert(offset >= 0);
  vector<i64> ans(N);
  for (int i = 0; i < N; ++i) {
    int j = (i - target + offset + N) % N;
    cout << pos[j] << '\n';
  }
}