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

struct flower{
    int hi, bty;
};

int solve(int n, vector<flower> &f){
    vi dp(n, 0);
    dp[0] = f[0].bty;
    mii mp;
    mp[f[0].hi] = dp[0];
    for(int i=1;i<n;i++){
        dp[i] = f[i].bty;
        auto it = mp.lower_bound(f[i].hi+1);
        if(it!=mp.begin()) {
            it--;
            dp[i] += it->second;
        }
        mp[f[i].hi] = dp[i];
        it = mp.upper_bound(f[i].hi);
        while(it!=mp.end() && it->second <= dp[i]){
            auto temp = it;
            temp++;
            mp.erase(it);
            it = temp;
        }
    }
    int ans = 0;
    for(int i=0;i<n;i++) ans = max(ans, dp[i]);
    return ans;
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n; cin>>n;
    int h[n], a[n];
    for(int i=0;i<n;i++) cin>>h[i];
    for(int i=0;i<n;i++) cin>>a[i];
    vector<flower> f(n);
    for(int i=0;i<n;i++){
        f[i].hi = h[i];
        f[i].bty = a[i];
    }
    cout<<solve(n, f)<<"\n";

    return 0;
}
