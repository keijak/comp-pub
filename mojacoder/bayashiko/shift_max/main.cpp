#include <bits/stdc++.h>
#include <atcoder/string>

using namespace std;
int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  string D;
  cin >> n >> D;
  string DD = D + D;
  auto sa = atcoder::suffix_array(DD);
  cout << DD.substr(sa.back(), n) << "\n";
}
