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

template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
struct SlideMinQueue {
  const Container& values;
  int left;
  int right;
  std::deque<int> que;
  const Compare compare;

  explicit SlideMinQueue(const Container& values)
      : values(values), left(0), right(0) {}

  void slide(int l, int r) {
    assert(l >= left);
    assert(r >= right);
    pop_left(l);
    left = l;
    for (int i = right; i < r; ++i) {
      push_right(i);
    }
    right = r;
  }

  T fold() const {
    assert(!empty());
    return que.front();
  }

  bool empty() const { return que.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!que.empty() && !compare(values[que.back()], values[i])) {
      que.pop_back();
    }
    que.emplace_back(i);
  }

  // Dequeues indices less than i.
  void pop_left(int i) {
    while (!que.empty() && que.front() < i) {
      que.pop_front();
    }
  }
};

template <class T, class Container = std::vector<T>>
using SlideMaxQueue = SlideMinQueue<T, Container, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N, K;
  cin >> N >> K;
  vector<i64> P(N);
  for (auto& x : P) cin >> x;

  vector<int> lsorted(N), rsorted(N);
  for (int i = 0; i < N; ++i) {
    lsorted[i] = 1;
    if (i > 0 && P[i - 1] <= P[i]) {
      lsorted[i] = lsorted[i - 1] + 1;
    }
    int j = N - 1 - i;
    rsorted[j] = 1;
    if (i > 0 && P[j] <= P[j + 1]) {
      rsorted[j] = rsorted[j + 1] + 1;
    }
  }

  SlideMinQueue<i64> minq(P);
  SlideMaxQueue<i64> maxq(P);
  set<tuple<int, int>> sorted_ranges;
  bool nochange = false;
  for (int l = 0; l <= N - K; ++l) {
    int r = l + K;  // exclusive
    assert(r <= N);
    minq.slide(l, r);
    maxq.slide(l, r);

    if (r - lsorted[r - 1] <= l) {
      DEBUG(nochange, l, r);
      nochange = true;
      continue;
    }

    i64 mine = P[minq.fold()];
    i64 maxe = P[maxq.fold()];
    int al = l;
    if (l - 1 >= 0 && P[l - 1] < mine) {
      al = l - lsorted[l - 1];
    }
    int ar = r;
    if (r < N && P[r] > maxe) {
      ar = r + rsorted[r];
    }
    sorted_ranges.emplace(al, ar);
  }
  int ans = sorted_ranges.size() + int(nochange);
  cout << ans << endl;
}
