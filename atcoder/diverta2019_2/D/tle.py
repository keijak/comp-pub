import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())

N = ni()
g1, s1, b1 = nm()
g2, s2, b2 = nm()


def solve():
    # Start
    don = N
    # A
    don_max = N
    for kg in range(don // g1 + 1):
        don1 = don - kg * g1
        temp1 = kg * g2
        for ks in range(don1 // s1 + 1):
            don2 = don1 - ks * s1
            temp2 = ks * s2
            if b2 > b1:
                kb = don2 // b1
                don3 = don2 - kb * b1
                temp3 = kb * b2
            else:
                kb = 0
                don3 = don2
                temp3 = 0
            if don_max < don3 + temp1 + temp2 + temp3:
                don_max = don3 + temp1 + temp2 + temp3
                # print(f"1> don_max={don_max} g:{kg} s:{ks} b:{kb}")

    don = don_max
    print(f">> at B: {don_max}", file=sys.stderr)
    for kg in range(don // g2 + 1):
        don1 = don - kg * g2
        temp1 = kg * g1
        for ks in range(don1 // s2 + 1):
            don2 = don1 - ks * s2
            temp2 = ks * s1
            if b1 > b2:
                kb = don2 // b2
                don3 = don2 - kb * b2
                temp3 = kb * b1
            else:
                kb = 0
                don3 = don2
                temp3 = 0
            if don_max < don3 + temp1 + temp2 + temp3:
                don_max = don3 + temp1 + temp2 + temp3
                # print(f"2> don_max={don_max} g:{kg} s:{ks} b:{kb}")
    return don_max


print(solve())
