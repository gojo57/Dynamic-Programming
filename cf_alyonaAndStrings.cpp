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

int n, m, k;
int dp[1001][1001][11][2];
string s, t;

int solve(int is, int it, int rem, int cont){
    if(is==n || it==m) {
        if(rem==0) return 0;
        return INT_MIN;
    }
    if(rem==0) return 0;
    if(dp[is][it][rem][cont] != -1) return dp[is][it][rem][cont];
    int ans = INT_MIN;
    if(cont==1){
        if(s[is]==t[it]){
            ans = max(ans, 1 + solve(is+1, it+1, rem, 1));
            ans = max(ans, 1 + solve(is+1, it+1, rem-1, 0));
        }
        ans = max(ans, solve(is+1, it, rem-1, 0));
        ans = max(ans, solve(is, it+1, rem-1, 0));
    }
    else{
        if(s[is]==t[it]){
            ans = max(ans, 1 + solve(is+1, it+1, rem, 1));
            ans = max(ans, 1 + solve(is+1, it+1, rem-1, 0));
        }
        ans = max(ans, solve(is+1, it, rem, 0));
        ans = max(ans, solve(is, it+1, rem, 0));
    }
    return dp[is][it][rem][cont] = ans;
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin>>n>>m>>k;
    cin>>s>>t;
    memset(dp, -1, sizeof(dp));
    cout<<solve(0, 0, k, 0)<<"\n";
    // memset(dp, 0, sizeof(dp));
    // for(int i = 1; i<=n; i++){
    //     for(int j = 1; j<=m; j++){
    //         for(int l = 1; l<=k; l++){
    //             if(s[i-1] == t[j-1]){
    //                 dp[i][j][l][1] = max(dp[i][j][l][1], dp[i-1][j-1][l][1] + 1);
    //                 dp[i][j][l][1] = max(dp[i][j][l][1], dp[i-1][j-1][l-1][0] + 1);
    //             }
    //             dp[i][j][l][0] = max(dp[i][j][l][0], dp[i-1][j][l][0]);
    //             dp[i][j][l][0] = max(dp[i][j][l][0], dp[i][j-1][l][0]);
    //             dp[i][j][l][0] = max(dp[i][j][l][0], dp[i][j][l][1]);
    //         }
    //     }
    // }
    // cout<<dp[n][m][k][0]<<"\n";
    return 0;
}
