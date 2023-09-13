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

//dp(i, g+1) = dp(i, g) + dp(i+1, g), opr = '<'
//dp(i, g+1) = dp(i, g) - dp(i+1, g), opr = '>'
//dp(i, 0) = sum dp(i+1, s-j), j=1 to s if opr = '>'
//dp(n, 0) = 1 if opr = '>' else 0

int dp[3001][3001];   
void solve(string &s, int n){
    dp[n][0] = (s[n-2] == '>' ? 1 : 0);
    dp[n][1] = (s[n-2] == '<' ? 1 : 0);
    for(int i = n-1; i>=2; i--){
        int tot = n-i+1;
        for(int g = 0; g<=tot; g++){
            if(g==0){
                if(i==n) continue;
                dp[i][g] = 0;
                if(s[i-2]=='<') continue;
                for(int j = 1; j<=tot; j++){
                    dp[i][g] = (dp[i][g] + dp[i+1][tot-j])%mod;
                }
            }
            else{
                if(s[i-2] == '>'){
                    dp[i][g] = (mod+dp[i][g-1] - dp[i+1][g-1])%mod;
                }
                else{
                    dp[i][g] = (dp[i][g-1] + dp[i+1][g-1])%mod;
                }
            }
        }
    }
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n; cin>>n;
    string s; cin>>s;
    int ans = 0;
    solve(s, n);
    for(int i = n-1; i>=0; i--){
        ans = (ans + dp[2][i])%mod;
    }
    cout<<ans<<"\n";

    return 0;
}
