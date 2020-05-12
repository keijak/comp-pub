x, y, a, b, c = map(int, input().split())
pa = list(map(int, input().split()))
pb = list(map(int, input().split()))
pc = list(map(int, input().split()))
pa.sort(reverse=True)
pb.sort(reverse=True)
pc.sort(reverse=True)
ia = ib = ic = psum = 0
for i in range(x+y):
    xa = pa[ia] if ia < len(pa) and ia < x else -1
    xb = pb[ib] if ib < len(pb) and ib < y else -1
    xc = pc[ic] if ic < len(pc) else -1
    if xa >= xb and xa >= xc:
        ia += 1
        psum += xa
    elif xb >= xc:
        ib += 1
        psum += xb
    else:
        ic += 1
        psum += xc
print(psum)
