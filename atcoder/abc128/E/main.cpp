#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
  int N, Q;
  cin >> N >> Q;
  vector<tuple<ll, bool, ll>> events;
  for (int i = 0; i < N; ++i) {
    ll s, t, x;
    cin >> s >> t >> x;
    events.emplace_back(s - x, true, x);
    events.emplace_back(t - x, false, x);
  }
  sort(events.begin(), events.end());
  int j = 0;
  set<ll> xs;
  for (int i = 0; i < Q; ++i) {
    ll d;
    cin >> d;
    for (;; ++j) {
      if (j == events.size()) {
        cout << -1 << endl;
        break;
      }
      auto [t, to_add, x] = events[j];
      if (t > d) {
        if (xs.empty()) {
          cout << -1 << endl;
        } else {
          cout << *xs.begin() << endl;
        }
        break;
      }
      if (to_add) {
        xs.insert(x);
      } else {
        xs.erase(x);
      }
    }
  }
}
