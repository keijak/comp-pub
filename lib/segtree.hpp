#include <iostream>
#include <vector>

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

  explicit SegTree(int n) : size_(n), data_(2 * n, Monoid::unity()) {}

  inline int size() const { return size_; }

  // Initializes the data array.
  void init(const std::vector<T> &leaves) {
    assert(int(leaves.size()) == size_);
    for (int i = 0; i < size_; ++i) {
      data_[size_ + i] = leaves[i];
    }
    for (int k = size_ - 1; k > 0; --k) {
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const T &x) {
    int k = size_ + i;
    data_[k] = x;
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l += size_;
    r += size_;
    T vleft = Monoid::unity(), vright = Monoid::unity();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  T operator[](int i) const { return data_[size_ + i]; }

  template <bool (*pred)(const T &)>
  int max_right(int l) {
    return max_right(l, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int max_right(int l, Predicate pred) {
    assert(0 <= l && l <= size_);
    assert(predicate(Monoid::unity()));
    if (l == size_) return size_;
    l += size_;
    T sm = Monoid::unity();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!pred(Monoid::op(sm, data_[l]))) {
        while (l < size_) {
          l = (2 * l);
          if (pred(Monoid::op(sm, data_[l]))) {
            sm = Monoid::op(sm, data_[l]);
            l++;
          }
        }
        return l - size_;
      }
      sm = Monoid::op(sm, data_[l]);
      l++;
    } while ((l & -l) != l);
    return size_;
  }

  template <bool (*pred)(const T &)>
  int min_left(int r) {
    return min_left(r, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int min_left(int r, Predicate pred) {
    assert(0 <= r && r <= size_);
    assert(pred(Monoid::unity()));
    if (r == 0) return 0;
    r += size_;
    T sm = Monoid::unity();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!pred(Monoid::op(data_[r], sm))) {
        while (r < size_) {
          r = (2 * r + 1);
          if (pred(Monoid::op(data_[r], sm))) {
            sm = Monoid::op(data_[r], sm);
            r--;
          }
        }
        return r + 1 - size_;
      }
      sm = Monoid::op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

 private:
  int size_;
  std::vector<T> data_;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const SegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size_; ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}
