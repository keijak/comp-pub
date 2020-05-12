from collections import Counter
M = 10**9 + 7
N = int(input())
A = list(map(int, input().split()))
ac = Counter(A)

def validate():
    uniq = N // 2
    if N % 2 == 1:
        uniq += 1
        if ac[0] != 1:
            return False
    if len(ac) != uniq:
        return False
    for k, v in ac.items():
        if k == 0 and v == 1:
            continue
        if v != 2:
            return False
        if N % 2 == k % 2:
            return False
    return True


def solve():
    if not validate():
        return 0
    return pow(2, N//2, M)


print(solve())
