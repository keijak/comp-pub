#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

using State = V<int8_t>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int H, W;
  cin >> H >> W;
  State grid(H * W);
  REP(i, H) REP(j, W) cin >> grid[i * W + j];

  map<State, int> st_id;
  State st(H * W);
  REP(i, H * W) { st[i] = i; }
  int st_counter = 0;
  do {
    st_id[st] = st_counter++;
  } while (next_permutation(st.begin(), st.end()));
}
