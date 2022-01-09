#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

struct Point {
  using T = int;
  T x, y;
  int index;
};

template<class P>
struct KdTree2d {
  using T = typename P::T;
  std::vector<P> points_;

  struct Node {
    T xmin, xmax, ymin, ymax;
    std::vector<int> indices;
    Node *l, *r;

    Node(typename std::vector<P>::iterator begin,
         typename std::vector<P>::iterator end)
        : xmin(std::numeric_limits<T>::max()),
          xmax(std::numeric_limits<T>::lowest()),
          ymin(std::numeric_limits<T>::max()),
          ymax(std::numeric_limits<T>::lowest()) {
      if (begin == end) return;

      {
        auto[mn, mx] = std::minmax_element(
            begin, end, [](const P &a, const P &b) { return a.x < b.x; });
        xmin = mn->x;
        xmax = mx->x;
      }
      {
        auto[mn, mx] = std::minmax_element(
            begin, end, [](const P &a, const P &b) { return a.y < b.y; });
        ymin = mn->y;
        ymax = mx->y;
      }
      const int n = int(end - begin);
      indices.reserve(n);
      for (auto it = begin; it != end; ++it) {
        indices.emplace_back(it->index);
      }
      if (n == 1) return;

      auto mid = begin + (n >> 1);
      if (xmax - xmin >= ymax - ymin) {
        std::nth_element(begin, mid, end,
                         [](const P &a, const P &b) { return a.x < b.x; });
      } else {
        std::nth_element(begin, mid, end,
                         [](const P &a, const P &b) { return a.y < b.y; });
      }
      l = new Node(begin, mid);
      r = new Node(mid, end);
    }

    template<class F>
    void search_rect(T x_low, T y_low, T x_high, T y_high, F process_index) {
      static_assert(std::is_invocable_v<F, int>);
      // The node range is completely contained by the query range.
      if (x_low <= xmin and xmax <= x_high and y_low <= ymin and
          ymax <= y_high) {
        for (int i: indices) process_index(i);
        return;
      }
      // The node range is completely outside of the query range.
      if (xmax < x_low or x_high < xmin or ymax < y_low or y_high < ymin) {
        return;
      }
      // Otherwise
      if (l) l->search_rect(x_low, y_low, x_high, y_high, process_index);
      if (r) r->search_rect(x_low, y_low, x_high, y_high, process_index);
    }
  };
  Node root_;

  explicit KdTree2d(std::vector<P> points)
      : points_(std::move(points)), root_(points_.begin(), points_.end()) {}

  // Visit each point in (top-left x bottom-right) rectangle (boundary
  // inclusive).
  template<class F>
  void search_rect(T x_low, T y_low, T x_high, T y_high, F process_index) {
    root_.search_rect(x_low, y_low, x_high, y_high, process_index);
  }
};

using namespace std;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Point> points(n);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    points[i] = {x, y, i};
  }
  KdTree2d<Point> tree(points);
  int q;
  cin >> q;
  REP(i, q) {
    int sx, tx, sy, ty;
    cin >> sx >> tx >> sy >> ty;
    vector<int> res;
    tree.search_rect(sx, sy, tx, ty, [&](int j) {
      res.push_back(j);
    });
    sort(ALL(res));
    for (int j: res) {
      cout << j << '\n';
    }
    cout << '\n';
  }
}
