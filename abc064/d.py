N = int(input())
S = list(input())
assert len(S) == N

def solve():
    i = 0
    opens = []
    while i < len(S):
        if S[i] == '(':
            opens.append(i)
        elif opens:
            opens.pop()
        else:
            S.insert(0, '(')
            i += 1
        i += 1
    S.extend(')' * len(opens))

solve()
print(''.join(S))