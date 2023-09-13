#include<bits/stdc++.h>
#define pb push_back
#define all(x) x.begin(),x.end()
#define rall(x) x.rbegin(),x.rend()
#define ones(x) __builtin_popcountll(x)
#define int int
#define pii pair<int,int>
#define mii map<int,int>
#define vi vector<int>
#define vvi vector<vector<int>>
#define vpii vector<pair<int, int>>
using namespace std;
const int mod = 1e9+7;

int change(int amount, vector<int>& coins) {
    int row=coins.size()+1;
    int col=amount+1;
    vector<vector<int> >dp(row,vector<int>(col,-1));
    for(int i=0;i<row;i++){
        dp[i][0]=1;
    }
    for(int i=1;i<col;i++){
        dp[coins.size()][i]=0;
    }
    
    for(int i=coins.size()-1;i>=0;i--){
        for(int j=1;j<=amount;j++){
            int count=0;
            if(coins[i]<=j){
                count = (count + dp[i][j-coins[i]])%mod;
            }
            count = (count + dp[i+1][j])%mod;
                dp[i][j]=count;
        }
        
    }
    return dp[0][amount];
}

int solve(int ind, int val, vector<int>& denominations, vector<vector<int>> &dp){
    if(ind==0){
        return val%denominations[ind]==0;
    }
    if(dp[ind][val]!=-1) return dp[ind][val];
    int nTake = solve(ind-1, val, denominations, dp)%mod;
    int take = 0;
    if(denominations[ind]<=val) take = solve(ind, val-denominations[ind], denominations, dp)%mod;
    return dp[ind][val] = (nTake+take)%mod;
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int test; cin>>test;
    while(test--){
        int n; cin>>n;
        vector<int> v(n);
        for(int i = 0; i<n; i++){
           cin>>v[i];
        }
        int target; cin>>target;
        cout<<change(target, v)<<"\n";
        // vvi dp(n, vi(target+1, -1));
        // cout<<solve(n-1, target, v, dp)<<"\n";
    }
    return 0;
}
