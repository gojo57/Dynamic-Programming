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

int fact[200001];
void preprocess(){
    fact[0] = 1;
    for(int i=1;i<200001;i++){
        fact[i] = (fact[i-1]*i)%mod;
    }
}

int power(int a, int b){
    int ans = 1;
    while(b){
        if(b&1) ans = (ans*a)%mod;
        a = (a*a)%mod;
        b >>= 1;
    }
    return ans;
}

int inverse(int a){  // a mod m = a^(m-2) mod m by fermat's little theorem.
    return power(a, mod-2);
}

int ncrmod(int n, int r){
    if(r>n) return 0;
    int ans = fact[n];
    ans = (ans*inverse(fact[r]))%mod;
    ans = (ans*inverse(fact[n-r]))%mod;
    return ans;
}

// ans = total paths - sum of the paths blocked by Oi but not by Oj for all i<j (O is an obstacle).
// we will sort the obstacles by their manhattan distances from (1,1).

int pathBwPoints(pii p1, pii p2){
    if(p1.first <= p2.first && p1.second<=p2.second) {
        int down = p2.first-p1.first;
        int right = p2.second-p1.second;
        return ncrmod(down+right, down);
    }
    return 0;
}

int solve(int h, int w, vpii &obs){
    int total = ncrmod(h+w-2, h-1);
    sort(all(obs), [&](pii a, pii b){  // sorting by the manhattan distance from (1,1).
        return a.first+a.second < b.first+b.second;
    });
    vector<int> pathsToObs;
    pathsToObs.pb(pathBwPoints({1,1}, obs[0]));
    for(int i=1;i<obs.size();i++){
        int paths = pathBwPoints({1,1}, obs[i]);
        for(int j = i-1; j>=0; j--){
            paths = (mod + paths - (pathsToObs[j]*pathBwPoints(obs[j], obs[i]))%mod)%mod;
        }
        pathsToObs.pb(paths);
    }
    for(int i=0;i<obs.size();i++){
        total = (mod + total - (pathsToObs[i]*pathBwPoints(obs[i], {h, w}))%mod)%mod;
    }
    return total;
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int h, w, n; cin >> h >> w >> n;
    preprocess();
    vpii obs(n);
    for(int i=0;i<n;i++){
        cin >> obs[i].first >> obs[i].second;
    }
    cout<<solve(h, w, obs);
    
    return 0;
}
