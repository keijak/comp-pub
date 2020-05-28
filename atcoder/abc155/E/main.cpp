#include <iostream>
#include <string>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  string ns;
  getline(cin, ns);
  int a = 0, b = 1;
  for (auto c : ns) {
    int d = c - '0';
    int a_ = min(a + d, b + 10 - d);
    b = min(a + d + 1, b + 9 - d);
    a = a_;
  }
  cout << a << '\n';
}
