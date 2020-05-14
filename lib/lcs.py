# Longest Common Subsequence
def lcs(s1, s2):
    if not s1 or not s2:
        return 0
    dp = [[0] * len(s2) for _ in s1]
    dp[0][0] = int(s1[0] == s2[0])
    for i in range(1, len(s2)):
        dp[0][i] = max(dp[0][i - 1], int(s1[0] == s2[i]))
    for i in range(1, len(s1)):
        dp[i][0] = max(dp[i - 1][0], int(s1[i] == s2[0]))
        for j in range(1, len(s2)):
            t = max(dp[i - 1][j], dp[i][j - 1])
            dp[i][j] = max(t, dp[i - 1][j - 1] + int(s1[i] == s2[j]))
    return dp[-1][-1]
