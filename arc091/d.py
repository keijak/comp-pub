N,K=map(int,input().split())
print(sum(N//b*(b-K) + max(N%b+1-K,0) - (K<1) for b in range(K+1,N+1)))
