struct ZAlgorithm {
  const int N;
  vector<int> Z;
  const string_view S;

  explicit ZAlgorithm(string_view s) : N(s.size()), Z(s.size()), S(move(s)) {}

  void calc() {
    Z[0] = N;
    int i = 1, j = 0;
    while (i < N) {
      while (i + j < N and S[j] == S[i + j]) ++j;
      Z[i] = j;
      if (j == 0) {
        ++i;
        continue;
      }
      int k = 1;
      for (; k < j and k + Z[k] < j; ++k) {
        Z[i + k] = Z[k];
      }
      i += k;
      j -= k;
    }
  }
};
