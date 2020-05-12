N=int(input());t={0:0}
for i in range(1,N+1):
 j=0
 while 6**j<=i:t[i]=min(t.get(i,N),t[i-6**j]+1,t.get(i-9**j,N)+1);j+=1
print(t[N])