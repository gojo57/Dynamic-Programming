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

////****************************segtree//****************************//

int lazy[4*N+1];
int segTree[4*N+1];
int a[N];

void build(int v, int tl, int tr)
{
    if(tl==tr){
        segTree[v] = a[tl];
    }
    else{
        int tm = (tl+tr)/2;
        build(2*v,tl,tm);
        build(2*v+1,tm+1,tr);
        // segTree[v] = segTree[2*v] + segTree[2*v+1];
    }
}

void updateRangeLazy(int ss, int se, int l, int r, int inc, int node){  // O(logn)
    //First step - Never go down if you have lazy value at node, first resolve it
    if(lazy[node]!=0){
        segTree[node] = lazy[node];
        //if not a leaf node
        if(ss!=se){ // pass the lazy value to children
            lazy[2*node] = lazy[node];
            lazy[2*node+1] = lazy[node];
        }
        lazy[node] = 0;
    }

    //Out of bounds
    if(l>se || r<ss){
        return;
    }

    //Complete Overlap
    if(ss>=l && se<=r){
        segTree[node] = inc;
        //Pass the lazy value to children
        if(ss!=se){
            lazy[2*node] = inc;
            lazy[2*node+1] = inc;
        }
        return;
    }

    //Partial Overlap
    int mid = (ss+se)/2;
    updateRangeLazy(ss, mid, l, r, inc, 2*node);
    updateRangeLazy(mid+1, se, l, r, inc, 2*node+1);

    segTree[node] = segTree[2*node]+ segTree[2*node+1];
    return;
}

int queryLazy(int *tree, int ss, int se, int qs, int qe, int index){  // O(logn)
    //First step - Never go down if you have lazy value at node, first resolve it
    if(lazy[index]!=0){
        tree[index] = lazy[index];
        //if not a leaf node
        if(ss!=se){ // pass the lazy value to children
            lazy[2*index] = lazy[index];
            lazy[2*index+1] = lazy[index];
        }
        lazy[index] = 0;
    }

    //Complete Overlap
    if(ss>=qs && se<=qe){
        return tree[index];
    }

    //No Overlap
    if(qe<ss || qs>se){
        return INT_MAX;
    }

    //Partial Overlap
    int mid = (ss+se)/2;
    int left = queryLazy(tree, ss, mid, qs, qe, 2*index);
    int right = queryLazy(tree, mid+1, se, qs, qe, 2*index+1);

    return min(left, right); 
}

void printUpdatedArray(int node, int tl, int tr) {
    if(lazy[node]!=0){
        segTree[node] = lazy[node];
        //if not a leaf node
        if(tl!=tr){ // pass the lazy value to children
            lazy[2*node] = lazy[node];
            lazy[2*node+1] = lazy[node];
        }
        lazy[node] = 0;
    }
    if (tl == tr) {
        cout << segTree[node] << "\n";
    } else {
        int tm = (tl + tr) / 2;
        printUpdatedArray(node * 2, tl, tm);
        printUpdatedArray(node * 2 + 1, tm + 1, tr);
    }
}

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

int solve(int *wt, int *val, int n, int w, vvi &dp){
    if(n==0 || w==0) return 0;
    if(dp[n][w] != -1) return dp[n][w];
    if(wt[n-1]<=w){
        return dp[n][w] = max(val[n-1]+solve(wt, val, n-1, w-wt[n-1], dp), solve(wt, val, n-1, w, dp));
    }
    else return dp[n][w] = solve(wt, val, n-1, w, dp);
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, w; cin>>n>>w;
    int wt[n], val[n];
    for(int i=0;i<n;i++) cin>>wt[i]>>val[i];
    vvi dp(n+1, vi(w+1, -1));
    cout<<solve(wt, val, n, w, dp)<<"\n";
    // for(int i=0;i<=n;i++){
    //     for(int j=0;j<=w;j++){
    //         if(i==0 || j==0) dp[i][j] = 0;
    //         else if(wt[i-1]<=j){
    //             dp[i][j] = max(val[i-1]+dp[i-1][j-wt[i-1]], dp[i-1][j]);
    //         }
    //         else dp[i][j] = dp[i-1][j];
    //     }
    // }
    // cout<<dp[n][w]<<"\n";
    return 0;
}
