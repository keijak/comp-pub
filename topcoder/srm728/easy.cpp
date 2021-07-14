#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>
using namespace std;

struct Halving {
  int n;

  int minSteps(vector<int> a) {
    n = a.size();
    map<int, pair<int, int>> counter;
    int ans = 1e9;
    for (int i = 0; i < n; ++i) {
      set<int> done;
      queue<pair<int, int>> q;

      auto push = [&](int x, int step) {
        if (done.count(x)) return;
        done.insert(x);
        auto it = counter.find(x);
        if (it == counter.end()) {
          counter[x] = {0, 0};
        }
        auto& p = counter[x];
        p.first++;
        p.second += step;
        if (p.first == n) {
          ans = min(ans, p.second);
        }
        q.push({x, step});
      };
      push(a[i], 0);

      while (q.size()) {
        int val, step;
        tie(val, step) = q.front();
        q.pop();
        vector<int> nx;
        if (val % 2 == 0) {
          nx.push_back(val / 2);
        } else {
          nx.push_back((val - 1) / 2);
          nx.push_back((val + 1) / 2);
        }
        for (int x : nx) {
          push(x, step + 1);
        }
      }
    }
    return ans;
  }
};

int main() {
  Halving s;
  int a;

  {
    a = s.minSteps({11, 4});
    cerr << a << endl;
    assert(a == 3);
  }
  {
    a = s.minSteps({1, 2, 3, 4, 5, 6, 7});
    cerr << a << endl;
    assert(a == 10);
  }
}
