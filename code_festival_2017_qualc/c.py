s = input()
N = len(s)

def solve():
    l = 0
    r = N - 1
    cost = 0
    while l < r:
        if s[l] == s[r]:
            l += 1
            r -= 1
        elif s[l] == 'x':
            cost += 1
            l += 1
        elif s[r] == 'x':
            cost += 1
            r -= 1
        else:
            return -1
    return cost

print(solve())
