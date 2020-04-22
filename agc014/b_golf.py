c=[0]*9**6
for x in open(0).read().split()[2:]:c[int(x)]^=1
print("YNEOS"[any(c)::2])
