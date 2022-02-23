#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct Interval {
  Int l, r;
  int val;
};
ostream &operator<<(ostream &os, const Interval &iv) {
  return os << "{[" << iv.l << ", " << iv.r << ") => " << iv.val << "}";
}

struct EventHandler {
  map<int, Int> val_counts;
  Int total = 0;

  EventHandler() = default;

  void on_add(const Interval &iv) {
    Int d = iv.r - iv.l;
    Int c = val_counts[iv.val];
    total += d * c + d * (d - 1) / 2;
    val_counts[iv.val] += d;
  }

  void on_remove(const Interval &iv) {
    Int d = iv.r - iv.l;
    Int c = val_counts[iv.val];
    total += -d * c + d * (d + 1) / 2;
    val_counts[iv.val] -= d;
  }
};

class IntervalMap : public std::map<Int, Interval> {
 public:
  EventHandler event_handler_;

  IntervalMap() = default;

  // Returns the interval which contains p if available.
  // Otherwise returns this->end().
  std::map<Int, Interval>::iterator find_interval(Int p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second.r > p) return it;
    }
    return end();
  }

  // Inserts interval [l, r)
  void add_interval(const Interval &interval) {
    remove_interval(interval.l, interval.r);
    event_handler_.on_add(interval);
    (*this)[interval.l] = interval;
  }

  // Removes interval [l, r)
  void remove_interval(Int l, Int r) {
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second.r <= l) ++itl;
    }
    if (itl == itr) return;
    std::optional<Interval> ltip, rtip;
    if (itl->first < l) {
      ltip = itl->second;
      assert(ltip->r >= l);
      ltip->r = l;
    }
    if (auto riv = std::prev(itr)->second; riv.r > r) {
      rtip = riv;
      assert(rtip->l <= r);
      rtip->l = r;
    }
    for (auto it = itl; it != itr; it = erase(it)) {
      event_handler_.on_remove(it->second);
    }
    if (ltip) {
      event_handler_.on_add(*ltip);
      (*this)[ltip->l] = *ltip;
    }
    if (rtip) {
      event_handler_.on_add(*rtip);
      (*this)[rtip->l] = *rtip;
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  IntervalMap im;
  REP(i, n) {
    im.add_interval(Interval{i, i + 1, a[i]});
  }
  const int Q = in;
  REP(qi, Q) {
    int l = in, r = in, x = in;
    --l;
    im.add_interval(Interval{l, r, x});
    cout << im.event_handler_.total << '\n';
  }
}
