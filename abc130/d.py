n,k = map(int,input().split())
a = [int(x) for x in input().split()]
assert len(a) == n


s0 = sum(a[:k])


print(a)