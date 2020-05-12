from numpy import*
x,y,n,*L=map(int,open(0).read().split());d=zeros((x+1,x+y+1),int)
for t,h in zip(*[iter(L)]*2):d[1:,t:]=maximum(d[1:,t:],d[:-1,:-t]+h)
print(d.max())