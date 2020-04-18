N = int(input())
a = [int(input()) for _ in range(N)]
print('first' if any(x % 2 for x in a) else 'second')
