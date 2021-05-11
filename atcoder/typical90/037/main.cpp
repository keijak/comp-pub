#include <bits/stdc++.h>
using Int = long long;

template <typename Monoid>
struct DualSegTree {
  using T = typename Monoid::T;

  explicit DualSegTree(int n) : size_(n), data_(2 * n, Monoid::id()) {}

  inline int size() const { return size_; }

  // Returns i-th value.
  T operator[](int i) const {
    int k = size_ + i;
    T res = data_[k];
    while (k > 1) {
      k >>= 1;
      res = Monoid::op(res, data_[k]);
    }
    return res;
  }

  // Applies a value to [l,r) range.
  void apply(int l, int r, T val) {
    l += size_;
    r += size_;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        data_[l] = Monoid::op(data_[l], val);
        ++l;
      }
      if (r & 1) {
        --r;
        data_[r] = Monoid::op(data_[r], val);
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const DualSegTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      os << st[i];
    }
    return os << "]";
  }

 private:
  int size_;
  std::vector<T> data_;
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
  DualSegTree<Max> dp1(W + 1), dp2(W + 1);
  dp1.apply(0, 1, 0LL);
  for (int i = 0; i < N; ++i) {
    Int l, r, value;
    cin >> l >> r >> value;
    dp2 = dp1;
    for (int j = 0; j <= W; ++j) {
      if (j + l > W) break;
      if (dp2[j] < 0) continue;
      dp1.apply(j + l, min(j + r, W) + 1, dp2[j] + value);
    }
  }
  return dp1[W];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
