_,a=open(0)
S=sorted(int(x)-i for i,x in enumerate(a.split()))
print(sum(abs(S[len(S)//2]-s)for s in S))