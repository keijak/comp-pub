#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
template<class Semigroup>
struct DisjointSparseTable {
  using value_type = typename Semigroup::T;
  using size_type = size_t;
  std::vector<std::vector<value_type>> table;

  explicit DisjointSparseTable(const std::vector<value_type> &seq) {
    const size_type size = seq.size();
    table.push_back(seq);  // table[0] = copy of the original sequence
    for (size_type i = 2; i < size; i <<= 1) {
      std::vector<value_type> v;
      v.reserve(size);
      for (size_type j = i; j < size; j += i << 1) {
        v.push_back(seq[j - 1]);
        for (size_type k = 2; k <= i; ++k) {
          v.push_back(Semigroup::op(seq[j - k], v.back()));
        }
        v.push_back(seq[j]);
        for (size_type k = 1; k < i && j + k < size; ++k) {
          v.push_back(Semigroup::op(v.back(), seq[j + k]));
        }
      }
      table.push_back(std::move(v));
    }
  }

  int size() const { return table.empty() ? 0 : (int) table.front().size(); }

  bool empty() const { return size() == 0; }

  // Folds the range [first, last). O(1).
  value_type fold(int first, int last) const {
    assert(first < last);
    assert(first >= 0);
    assert(last <= size());
    if (first + 1 == last) {
      return table.front()[first];
    } else {
      const size_type p = msb_log(first ^ (last - 1));
      return Semigroup::op(table[p][first ^ (size_type(1) << p) - 1],
                           table[p][last - 1]);
    }
  }

  const value_type &operator[](int index) const {
    assert(index < size());
    return table.front()[index];
  }

 private:
  template<typename T>
  static constexpr int num_bits = CHAR_BIT * sizeof(T);

  // Log base 2 of the most significant bit that is set to 1.
  static inline int msb_log(unsigned x) {
    assert(x != 0);
    return num_bits<unsigned> - __builtin_clz(x) - 1;
  }
};

struct MinOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
};

using namespace std;

int main() {
  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  for (auto &x: A) cin >> x;
  DisjointSparseTable<MinOp> st(A);
  REP(i, Q) {
    int l, r;
    cin >> l >> r;
    cout << st.fold(l, r) << endl;
  }
}
