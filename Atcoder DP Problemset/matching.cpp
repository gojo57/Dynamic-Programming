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

//****************************dsu****************************//
class Graph{
    int n, *rank, *parent;
public:
    Graph(int n){
        this->n = n;
        rank = new int[n];
        parent = new int[n];
        for(int i=0;i<n;i++){
            rank[i] = 1;
            parent[i] = i;
        }
    }
    int find(int x){
        if(parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }
    void union_set(int x, int y){
        x = find(x);
        y = find(y);
        if(x != y){
            if(rank[x] < rank[y]) swap(x, y);
            parent[y] = x;
            rank[x] += rank[y];
        }
    }
    int connected_components(){
        int ans = 0;
        for(int i=0;i<n;i++){
            if(parent[i] == i) ans++;
        }
        return ans;
    }
    ~Graph(){
        delete [] rank;
        delete [] parent;
    }
};

// dp[i][mask] = Number of ways to match first i men with the set of women indicated by the mask, 0 <= i <= n, 0 <= mask <= 2^n. Obviuosly, if i != popcount(mask), dp[i][mask] = 0.

// Fill the dp table by iterating over i and mask. When you fill dp[i][mask], you iterate over all possible pairing of i-th man with a woman. So the transition is dp[i][mask] = (sum of dp[i-1][mask ^ 1 << j] where j-th woman appeared in the mask).

int solve(vvi &a, vvi &dp, int n, int i, int mask){
    if(i == n) {
        if(mask == 0) return 1;
        return 0;
    }
    if(dp[i][mask] != -1) return dp[i][mask];
    int ans = 0;
    for(int j=0;j<n;j++){
        if(a[i][j] && (mask & (1<<j))){
            ans = (ans + solve(a, dp, n, i+1, mask ^ (1<<j))) % mod; // mask ^ (1<<j) is the mask with j-th bit flipped because we have to turn that bit off as the woman has been paired.
        }
    }
    return dp[i][mask] = ans;
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n; cin>>n;
    vvi a(n, vi(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>a[i][j];
        }
    }
    vvi dp(n, vi(1<<(n), -1));
    cout<<solve(a, dp, n, 0, (1<<(n))-1)<<"\n";

    return 0;
}
