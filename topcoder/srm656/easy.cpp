#include <bits/stdc++.h>

using namespace std;
using Float = long double;

struct RandomPancakeStack {
  template <class T = Float>
  std::vector<std::vector<T>> comb_table(int n) {
    std::vector<std::vector<T>> C(n + 1, std::vector<T>(n + 1, 0));
    for (int i = 0; i <= n; ++i) {
      C[i][0] = C[i][i] = 1;
    }
    for (int i = 1; i <= n; ++i) {
      for (int j = 1; j < i; ++j) {
        C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
      }
    }
    return C;
  }

  double expectedDeliciousness(vector<int> d) {
    const int n = d.size();
    vector<Float> z(n);
    z[n - 1] = Float(1) / n;
    for (int i = n - 2; i >= 0; --i) {
      z[i] = z[i + 1] / Float(i + 1);
    }
    const auto C = comb_table(n);
    Float ans = 0;
    for (int i = 0; i < n; ++i) {
      Float count = 0;
      for (int j = 0; j <= i; ++j) {
        count += C[i][j] * z[n - 1 - j];
      }
      ans += d[n - 1 - i] * count;
    }
    return (double)ans;
  }
};

int main() {
  cout << std::fixed << std::setprecision(20);
  RandomPancakeStack task;
  auto f = [&](vector<int> &&v) {
    cerr << (task.expectedDeliciousness(v)) << endl;
  };

  f({1, 1, 1});
  f({3, 6, 10, 9, 2});
  f({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
  f({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  f({2,  7, 1, 8, 2, 8, 1,  8, 2, 8,  4, 5,
     90, 4, 5, 2, 3, 5, 60, 2, 8, 74, 7, 1});
  f({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
}
