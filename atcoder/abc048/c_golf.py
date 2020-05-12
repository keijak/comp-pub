n,x,*a=map(int,open(0).read().split());t=r=0
for i in a:t=min(i,x-t);r+=i-t
print(r)