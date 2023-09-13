#include<bits/stdc++.h>
#define pb push_back
#define all(x) x.begin(),x.end()
#define rall(x) x.rbegin(),x.rend()
#define ones(x) __builtin_popcountll(x)
#define pii pair<int,int>
#define mii map<int,int>
#define vi vector<int>
#define vvi vector<vector<int>>
#define vpii vector<pair<int, int>>
using namespace std;
const int mod = 1e9+7;
const int N = 2e5+5;

int aa[1000];
int dp[1000][1000];

int sum(int s, int e){
    int ans = 0;
    for(int i = s; i<=e; i++){
        ans += aa[i];
        ans %= 100;
    }
    return ans;
}

int solve(int i, int j){
    if(i>=j) return 0;
    if(dp[i][j] != -1) return dp[i][j];
    dp[i][j] = INT_MAX;
    for(int k = i; k<=j; k++){
        dp[i][j] = min(dp[i][j], solve(i, k) + solve(k+1, j) + sum(i, k)*sum(k+1, j));
    }
    return dp[i][j];
}

//****************************main****************************//
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF){
        for(int i = 0; i<n; i++){
            cin>>aa[i];
        }
        for(int i = 0; i<n; i++){
            for(int j = 0; j<n; j++){
                dp[i][j] = -1;
            }
        }
        for(int i = 0; i<n; i++){
            dp[i][i] = 0;
        }
        cout<<solve(0, n-1)<<"\n";
    }
    return 0;
}
