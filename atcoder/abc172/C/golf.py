from bisect import*
from itertools import*
n,m,k,*x=map(int,open(0).read().split());c=accumulate;b=[*c(x[n:])];print(max(i+bisect(b,k-v)for i,v in enumerate(c([0]+x[:n]))if v<=k))