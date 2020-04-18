from bisect import*
(N,),A,B,C=[sorted(map(int,input().split()))for _ in range(4)]
w,i=[0]*(N+1),N
while i:i-=1;w[i]=w[i+1]+N-bisect(C,B[i])
print(sum(w[bisect(B,a)]for a in A))
