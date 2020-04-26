M=2019;t=1;p=r=0;c=[1]+[0]*M
for x in input()[::-1]:p=(p+int(x)*t)%M;t=t*10%M;r+=c[p];c[p]+=1
print(r)