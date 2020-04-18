N, X = map(int, input().split())
layers = [0] * (N+1)
layers[0] = 1
patties = [0] * (N+1)
patties[0] = 1
for i in range(1, N+1):
    layers[i] = 2*layers[i-1] + 3
    patties[i] = 2*patties[i-1] + 1

def eat(n, x):
    if x <= 0:
        return 0
    if n == 0:
        return 1
    mid = layers[n-1] + 2
    if x < mid:
        return eat(n-1, x-1)
    return 1 + patties[n-1] + eat(n-1, x-mid)

print(eat(N, X))
