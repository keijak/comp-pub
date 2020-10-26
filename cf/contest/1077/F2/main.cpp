#include <bits/stdc++.h>
using i64 = long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}

template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
struct SlideMinQueue {
  Container vals;
  Compare compare;              // comparison funcation.
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : compare(), left(0), right(0) {}
  SlideMinQueue(const SlideMinQueue &) = default;
  SlideMinQueue &operator=(const SlideMinQueue &) = default;
  SlideMinQueue(SlideMinQueue &&) = default;
  SlideMinQueue &operator=(SlideMinQueue &&) = default;

  explicit SlideMinQueue(Container v)
      : vals(std::move(v)), compare(), left(0), right(0) {}

  // Shift both `left` and `right` to the right.
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

  // Returns the minimum value in [left, right).
  const T &fold() const {
    assert(!empty());
    return vals[index_queue.front()];
  }

  bool empty() const { return index_queue.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!index_queue.empty() &&
           !compare(vals[index_queue.back()], vals[i])) {
      index_queue.pop_back();
    }
    index_queue.emplace_back(i);
  }

  // Dequeues indices less than i.
  void pop_left(int i) {
    while (!index_queue.empty() && index_queue.front() < i) {
      index_queue.pop_front();
    }
  }
};

template <class T, class Container = std::vector<T>>
using SlideMaxQueue = SlideMinQueue<T, Container, std::greater<T>>;

using namespace std;

i64 solve() {
  int n, k, x;
  cin >> n >> k >> x;
  vector<i64> a(n);
  REP(i, n) cin >> a[i];

  auto dp = vector(x + 1, SlideMaxQueue<i64>(vector<i64>(n + 1, -1)));
  dp[0].vals[0] = 0LL;
  REP(i, x + 1) { dp[i].slide(0, i + 1); }

  i64 ans = -1;
  for (int i = 0; i < n; ++i) {
    i64 beaut = a[i];
    for (int j = x - 1; j >= 0; --j) {
      dp[j].slide(max(i + 1 - k, 0), max(i + 1, j + 1));
      i64 val = dp[j].fold();
      if (val >= 0) {
        chmax(dp[j + 1].vals[i + 1], val + beaut);
      }
    }
    if (i >= n - k) {
      chmax(ans, dp[x].vals[i + 1]);
    }
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << endl;
}
