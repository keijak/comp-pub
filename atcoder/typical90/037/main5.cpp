#include <bits/stdc++.h>
using Int = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}

template <typename Monoid>
struct SWAGQueue {
  using T = typename Monoid::T;

  struct Node {
    T val, agg;
    Node(T val, T agg) : val(std::move(val)), agg(std::move(agg)) {}
  };
  std::stack<Node> front_stack, back_stack;

  SWAGQueue() = default;

  bool empty() const { return front_stack.empty() and back_stack.empty(); }

  size_t size() const { return front_stack.size() + back_stack.size(); }

  void push_back(const T &x) {
    if (back_stack.empty()) {
      back_stack.emplace(x, x);
    } else {
      back_stack.emplace(x, Monoid::op(back_stack.top().agg, x));
    }
  }

  void pop_front() {
    assert(!empty());
    if (front_stack.empty()) {
      front_stack.emplace(back_stack.top().val, back_stack.top().val);
      back_stack.pop();
      while (!back_stack.empty()) {
        T agg = Monoid::op(back_stack.top().val, front_stack.top().agg);
        front_stack.emplace(back_stack.top().val, std::move(agg));
        back_stack.pop();
      }
    }
    front_stack.pop();
  }

  T fold() const {
    if (empty()) return Monoid::id();
    if (front_stack.empty()) return back_stack.top().agg;
    if (back_stack.empty()) return front_stack.top().agg;
    return Monoid::op(front_stack.top().agg, back_stack.top().agg);
  }
};

struct Max {
  using T = Int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return -1LL; }
};

using namespace std;

Int solve() {
  Int W, N;
  cin >> W >> N;
  auto dp = vector(2, vector(W + 1, -1LL));
  dp[0][0] = 0;
  for (int i = 0; i < N; ++i) {
    Int l, r, value;
    cin >> l >> r >> value;
    dp[1] = dp[0];
    SWAGQueue<Max> sq;
    for (int j = l; j <= W; ++j) {
      sq.push_back(dp[1][j - l]);
      if (j - r - 1 >= 0) {
        sq.pop_front();
      }
      auto new_value = sq.fold();
      if (new_value >= 0) {
        chmax(dp[0][j], new_value + value);
      }
    }
  }
  return dp[0][W];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
