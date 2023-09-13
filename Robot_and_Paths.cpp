#include<bits/stdc++.h>
#define pb push_back
#define all(x) x.begin(),x.end()
#define rall(x) x.rbegin(),x.rend()
#define ones(x) __builtin_popcountll(x)
#define int long long
#define pii pair<int,int>
#define mii map<int,int>
#define vi vector<int>
#define vvi vector<vector<int>>
#define vpii vector<pair<int, int>>
using namespace std;
const int mod = 1e9+7;
const int N = 2e5+5;

int solve(vvi &dp, int m, int n){
    if(dp[0][0] == -1) return 0;
    for(int i = 0; i<m; i++){
        if(dp[i][0] == -1) break;
        dp[i][0] = 1;
    }
    for(int i = 0; i<n; i++){
        if(dp[0][i] == -1) break;
        dp[0][i] = 1;
    }
    for(int i = 1; i<m; i++){
        for(int j = 1; j<n; j++){
            if(dp[i][j] == -1) continue;
            dp[i][j] = 0;
            if(dp[i-1][j] != -1) dp[i][j] = (dp[i][j] + dp[i-1][j])%mod;
            if(dp[i][j-1] != -1) dp[i][j] = (dp[i][j] + dp[i][j-1])%mod;
        }
    }
    if(dp[m-1][n-1] == -1) return 0;
    return dp[m-1][n-1];
}

signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int m, n, p; cin >> m >> n >> p;
    vvi dp(m, vi(n, 0));
    for(int i = 0; i<p; i++){
        int x, y; cin >> x >> y;
        dp[x-1][y-1] = -1;
    }
    cout<<solve(dp, m, n)<<"\n";
    return 0;
}
