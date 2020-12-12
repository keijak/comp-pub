import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]


def solve():
    n = ini()
    a, b = inl()
    if a > b:
        return True
    if a < b:
        return n <= a
    return n % (a + 1) != 0


if __name__ == "__main__":
    print("Takahashi" if solve() else "Aoki")
