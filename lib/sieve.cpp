// Returns a bitset to tell if a number less than or equal to n is prime.
vector<bool> sieve(int n) {
  vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i <= n; i += 2) is_prime[i] = false;
  for (int i = 3; i * i <= n; i += 2) {
    if (!is_prime[i]) continue;
    for (int j = 2 * i; j <= n; j += i) is_prime[j] = false;
  }
  return is_prime;
}

// Returns the prime numbers less than or equal to n.
vector<int> primes(int n) {
  vector<int> res;
  vector<bool> is_prime = sieve(n);
  for (int i = 2; i <= n; ++i) {
    if (is_prime[i]) res.push_back(i);
  }
  return res;
}
