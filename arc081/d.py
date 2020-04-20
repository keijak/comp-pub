M = 1000000007
n = int(input())


def pack(s):
    p = [s[0]]
    for i in range(1, len(s)):
        if s[i] != s[i - 1]:
            p.append(s[i])
    return p


s1 = pack(input())
s2 = pack(input())


def solve():
    res = 6 if s1[0] != s2[0] else 3
    for i in range(1, len(s1)):
        if s1[i - 1] != s2[i - 1]:
            if s1[i] != s2[i]:
                res *= 3
            else:
                res *= 1
        else:
            if s1[i] != s2[i]:
                res *= 2
            else:
                res *= 2
        res %= M
    return res


print(solve())
