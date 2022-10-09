#include <iostream>
#include <string>
#include <atcoder/string>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::string S, T;
  std::cin >> n >> S >> T;
  const auto U = S + S + T + T;
  const auto sa = atcoder::suffix_array(U);
  const auto lcp = atcoder::lcp_array(U, sa);

  long long ans = 0, scnt = 0, ssum = 0;
  for (int i = (int) U.size() - 1; i >= 0; --i) {
    if (sa[i] < n) {
      ++scnt;
    } else if (2 * n <= sa[i] and sa[i] < 3 * n) {
      ans += n - ssum;
    }
    if (i > 0 and lcp[i - 1] < n) {
      ssum += scnt;
      scnt = 0;
    }
  }
  std::cout << ans << '\n';
}
