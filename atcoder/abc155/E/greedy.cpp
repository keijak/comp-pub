#include <iostream>
#include <string>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  string ns;
  getline(cin, ns);
  const int N = ns.size();
  int bills = 0, carry = 0;
  for (int i = N - 1; i >= 0; --i) {
    int d = (ns[i] - '0') + carry;
    carry = d / 10;
    d %= 10;
    if (d < 5) {
      bills += d;
    } else if (d == 5) {
      bills += d;
      carry = (i > 0) && (ns[i - 1] - '0') >= 5;
    } else {
      bills += 10 - d;
      carry = 1;
    }
  }
  bills += carry;
  cout << bills << '\n';
}
