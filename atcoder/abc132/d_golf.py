M=10**9+7;N,K=map(int,input().split())
def c(n,k,m=1,d=1):
 for i in range(k):m*=n-i;d*=i+1
 return m%M*pow(d%M,M-2,M)%M
for i in range(K):print(c(K-1,i)*(N-K>i-1and c(N-K+1,i+1))%M)
