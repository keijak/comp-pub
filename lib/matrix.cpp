// n * n Matrix
using MatrixRow = vector<long long>;
using Matrix = vector<MatrixRow>;

Matrix matmul(const Matrix& a, const Matrix& b) {
  int n = a.size();
  //   assert(int(a[0].size()) == n);
  //   assert(int(b.size()) == n);
  //   assert(int(b[0].size()) == n);

  Matrix ret(n, MatrixRow(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      for (int k = 0; k < n; ++k) {
        ret[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return ret;
}

Matrix matpow(const Matrix& a, long long k) {
  int n = a.size();
  //   assert(int(a[0].size()) == n);

  if (k == 0) {
    Matrix eye(n, MatrixRow(n));
    for (int i = 0; i < n; ++i) {
      eye[i][i] = 1;
    }
    return eye;
  }

  if (k == 1) {
    return a;
  }

  if (k & 1) {
    return matmul(matpow(a, k - 1), a);
  } else {
    return matpow(matmul(a, a), k / 2);
  }
}
