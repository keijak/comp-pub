import sys

input = sys.stdin.readline


def main():
    N = int(input())
    en = [[] for _ in range(N)]
    on = [[] for _ in range(N)]
    for i in range(N - 1):
        u, v, w = map(int, input().split())
        if w % 2:
            on[u - 1].append(v - 1)
            on[v - 1].append(u - 1)
        else:
            en[u - 1].append(v - 1)
            en[v - 1].append(u - 1)

    color = [-1] * N
    stack = [(0, -1, 0)]
    while stack:
        node, parent, col = stack.pop()
        if color[node] >= 0:
            continue
        color[node] = col
        for k in en[node]:
            if k != parent:
                stack.append((k, node, col))
        for k in on[node]:
            if k != parent:
                stack.append((k, node, (col + 1) % 2))

    print(*color, sep="\n")


if __name__ == "__main__":
    main()
