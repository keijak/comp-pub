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

const int DAY_END = 24 * 60;

int main() {
  int N;
  scanf("%d", &N);
  vector<tuple<int, int>> times(N);
  REP(i, N) {
    int h1, m1, h2, m2;
    scanf("%d:%d %d:%d", &h1, &m1, &h2, &m2);
    int t1 = h1 * 60 + m1;
    int t2 = h2 * 60 + m2;
    times[i] = {t2, t1};
  }
  sort(times.begin(), times.end());
  vector<vector<bool>> table(N, vector<bool>(DAY_END, true));
  auto available = [&](const vector<bool>& t, int start, int end) -> bool {
    if (end < DAY_END) {
      for (int i = start; i < end; ++i) {
        if (!t[i]) return false;
      }
    } else {
      for (int i = start; i < DAY_END; ++i) {
        if (!t[i]) return false;
      }
      for (int i = 0; i < end % (DAY_END); ++i) {
        if (!t[i]) return false;
      }
    }
    return true;
  };
  auto fill_table = [&](vector<bool>& t, int start, int end) {
    if (end < DAY_END) {
      for (int i = start; i < end; ++i) {
        t[i] = false;
      }
    } else {
      for (int i = start; i < DAY_END; ++i) {
        t[i] = false;
      }
      for (int i = 0; i < end % (DAY_END); ++i) {
        t[i] = false;
      }
    }
  };

  using Room = tuple<int, int, int>;
  multiset<Room> tset;
  int ans = 0;
  REP(i, N) {
    auto [t2, t1] = times[i];
    bool ok = false;
    for (auto [rlen, r1, r2] : tset) {
      if (t1 >= r1 && t2 <= r2) {
            }
    }
    if (!tset.empty()) {
    } else {
      if (t2 < DAY_END) {
        tset.emplace(t1 + DAY_END - t2, t2, t1);
      } else {
        tset.emplace(t1 - t2 % DAY_END, t2 % DAY_END, t1);
      }
    }

    for (int j = ans - 1; j >= 0; --j) {
      if (available(table[j], t1, t2)) {
        DEBUG(i, j, t1, t2);
        fill_table(table[j], t1, t2);
        goto loop_out;
      }
    }
    ans++;
    fill_table(table[ans - 1], t1, t2);
  loop_out:
    continue;
  }

  printf("%d\n", ans);
}
