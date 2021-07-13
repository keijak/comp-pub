#include <bits/stdc++.h>
using namespace std;

// Lowest Common Ancestor by EulerTour + RMQ (SparseTable).
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;
  vector<int> depth;
  vector<int> index;
  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)

  explicit EulerTour(G g, int root = 0)
      : n(g.size()), adj(move(g)), depth(n, 0), index(n, -1), tour() {
    tour.reserve(n * 2);
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    index[v] = int(tour.size());
    depth[v] = (p < 0) ? 0 : (depth[p] + 1);
    tour.emplace_back(depth[v], v);
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.emplace_back(depth[v], v);
    }
  }
};

using u64 = unsigned long long;

template<typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Most Significant Set Bit (Highest One Bit)
inline int mssb_pos(unsigned x) {
  assert(x != 0);
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
}
inline int mssb_pos(u64 x) {
  assert(x != 0);
  return std::numeric_limits<u64>::digits - __builtin_clzll(x) - 1;
}
template<typename T, typename U = std::make_unsigned_t<T>>
inline U mssb(T x) {
  if (x == 0) return 0;
  return U(1) << mssb_pos(U(x));
}
// Least Significant Set Bit (Lowest One Bit)
inline int lssb_pos(unsigned x) {
  assert(x != 0);
  return __builtin_ctz(x);
}
inline int lssb_pos(u64 x) {
  assert(x != 0);
  return __builtin_ctzll(x);
}
template<typename T, typename U = std::make_unsigned_t<T>>
inline U lssb(T x) {
  return (x & -x);
}
template<typename T, typename U = std::make_unsigned_t<T>>
inline U drop_lssb(T x) {
  return x & (x - 1);
}

template<class T, class BetterOp = less<T>, class mask_t = uint32_t>
struct RMQ {
  static_assert(std::is_integral<mask_t>::value, "mask_t must be integral");
  static_assert(std::is_unsigned<mask_t>::value, "mask_t must be unsigned");
  static_assert(std::is_invocable_r_v<bool, BetterOp, const T &, const T &>);
  static constexpr int block_size_ = num_bits<mask_t>;

  int n_;  // sequence size
  int block_count_;
  vector<T> data_;  // sequence data
  vector<mask_t> indicator_;
  vector<vector<int>> sparse_table_;
  BetterOp better_than_;  // (T lhs, T rhs) => true iff lhs is strictly better
  // than rhs.

  explicit RMQ(vector<T> data, BetterOp better = BetterOp())
      : n_(data.size()),
        block_count_((n_ + block_size_ - 1) / block_size_),
        data_(std::move(data)),
        indicator_(n_),
        sparse_table_(
            block_count_ == 0 ? 0 : mssb_pos(unsigned(block_count_)) + 1,
            vector<int>(block_count_)),
        better_than_(std::move(better)) {
    constexpr int qsize = block_size_ + 1;
    std::array<int, qsize> q;
    int lp = 0, rp = 0, rpm1 = qsize - 1;
    for (int r = 0; r < n_; ++r) {
      while (lp != rp and (r - q[lp] >= block_size_)) {
        ++lp;
        if (lp == qsize) lp = 0;
      }
      const auto &cur = data_[r];
      while (lp != rp and not better_than_(data_[q[rpm1]], cur)) {
        rp = rpm1--;
        if (rp == 0) rpm1 = qsize - 1;
      }
      mask_t ind = 1;
      if (lp != rp) {
        int p = q[rpm1];
        ind |= (indicator_[p] << (r - p));
      }
      indicator_[r] = ind;
      q[rp] = r;
      rpm1 = rp++;
      if (rp == qsize) rp = 0;
    }

    for (int i = 0; i < block_count_; ++i) {
      sparse_table_[0][i] =
          best_index_small(min(block_size_ * (i + 1), n_) - 1);
    }
    for (int i = 0; i + 1 < int(sparse_table_.size()); ++i) {
      for (int j = 0; j < block_count_; j++) {
        sparse_table_[i + 1][j] = better_index(
            sparse_table_[i][j],
            sparse_table_[i][std::min(j + (1 << i), block_count_ - 1)]);
      }
    }
  }

  // Returns the best value in [l, r).
  const T &query(int l, int r) const { return data_[best_index(l, r - 1)]; }

 private:
  int better_index(int i, int j) const {
    return better_than_(data_[i], data_[j]) ? i : j;
  }

  int best_index_small(int r, int sz = block_size_) const {
    assert(r < n_);
    assert(sz > 0);
    assert(sz <= block_size_);
    mask_t ind = indicator_[r];
    if (sz < block_size_) {
      ind &= (mask_t(1) << sz) - 1;
    }
    return r - mssb_pos(ind);
  }

  int best_index(int l, int r) const {
    l = std::clamp(l, 0, n_ - 1);
    r = std::clamp(r, 0, n_ - 1);
    if (r - l + 1 <= block_size_) {
      return best_index_small(r, r - l + 1);
    }
    int ql = best_index_small(std::min(l + block_size_, n_) - 1);
    int qr = best_index_small(r);
    l = l / block_size_ + 1;
    r = r / block_size_ - 1;
    int ans = better_index(ql, qr);
    if (l <= r) {
      int i = mssb_pos(unsigned(r - l + 1));
      int qs =
          better_index(sparse_table_[i][l], sparse_table_[i][r - (1 << i) + 1]);
      ans = better_index(ans, qs);
    }
    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  vector<vector<int>> g(n);
  for (int i = 0; i < n - 1; ++i) {
    int p;
    cin >> p;
    g[p].push_back(i + 1);
  }

  EulerTour et(move(g));
  RMQ<pair<int, int>> rmq(et.tour);

  for (int i = 0; i < q; ++i) {
    int u, v;
    cin >> u >> v;
    int ui = et.index[u], vi = et.index[v];
    auto[lca_depth, lca_id] = rmq.query(min(ui, vi), max(ui, vi) + 1);
    cout << lca_id << '\n';
  }
}
