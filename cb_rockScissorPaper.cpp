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
    //First step - Never go down if you have lazy value at node, first rerockSurvival it
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
    //First step - Never go down if you have lazy value at node, first rerockSurvival it
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

// https://codeforces.com/problemset/problem/540/D

// In RSS Shakha, kids play an interesting game called Rocks, Paper and Scissors. R number of rocks, P number of papers and S number of scissors are given. 
// You have to tell the probabilities pf rock being the only survivor of the game, paper being the only survivor of the game and scissors being the only survivor of the game.

// Input Format
// The first line of input contains an integer T denoting the number of test cases. Each test case contains three integers R, P and S denoting the number of rocks, papers and scissors respectively.

// Constraints
// 1 <= T <= 10
// 1 <= R, P, S <= 100

// Output Format
// For each test case, print the probabilities of rock, paper and scissors being the only survivor of the game. Print the answer upto 9 decimal places.

// Sample Input
// 3
// 2 2 2
// 2 1 2
// 1 1 3

// Sample Output
// 0.333333333 0.333333333 0.333333333
// 0.150000000 0.300000000 0.550000000
// 0.057142857 0.657142857 0.285714286

double dp[105][105][105]; // dp[r][s][p] = probability of rock being the only survivor when r rocks, s scissors and p papers are there
void set_dp(double dp[105][105][105]){
    for(int i=0;i<105;i++){
        for(int j=0;j<105;j++){
            for(int k=0;k<105;k++){
                dp[i][j][k] = -1.0;
            }
        }
    }
}

double rockSurvival(int r, int s, int p){ // tells the probability of rock being the only survivor
    if(r==0 || s==0) return 0.0;
    if(p==0) return 1.0;
    if(dp[r][s][p]!=-1.0) return dp[r][s][p];
    double tot = r*s + s*p + p*r;
    double ans = 0.0;
    ans += (r*s)/tot * rockSurvival(r, s-1, p); // rock kills scissor
    ans += (s*p)/tot * rockSurvival(r, s, p-1); // scissor kills paper
    ans += (p*r)/tot * rockSurvival(r-1, s, p); // paper kills rock
    return dp[r][s][p] = ans;
}

double paperSurvival(int r, int s, int p){ // tells the probability of paper being the only survivor
    if(r==0 || p==0) return 0.0;
    if(s==0) return 1.0;
    if(dp[r][s][p]!=-1.0) return dp[r][s][p];
    double tot = r*s + s*p + p*r;
    double ans = 0.0;
    ans += (r*s)/tot * paperSurvival(r, s-1, p); // rock kills scissor
    ans += (s*p)/tot * paperSurvival(r, s, p-1); // scissor kills paper
    ans += (p*r)/tot * paperSurvival(r-1, s, p); // paper kills rock
    return dp[r][s][p] = ans;
}

double scissorSurvival(int r, int s, int p){ // tells the probability of scissor being the only survivor
    if(s==0 || p==0) return 0.0;
    if(r==0) return 1.0;
    if(dp[r][s][p]!=-1.0) return dp[r][s][p];
    double tot = r*s + s*p + p*r;
    double ans = 0.0;
    ans += (r*s)/tot * scissorSurvival(r, s-1, p); // rock kills scissor
    ans += (s*p)/tot * scissorSurvival(r, s, p-1); // scissor kills paper
    ans += (p*r)/tot * scissorSurvival(r-1, s, p); // paper kills rock
    return dp[r][s][p] = ans;
}

//****************************main****************************//
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int r, s, p; cin>>r>>s>>p;
    set_dp(dp);
    double rock = rockSurvival(r, s, p);
    set_dp(dp);
    double paper = paperSurvival(r, s, p);
    set_dp(dp);
    double scissor = scissorSurvival(r, s, p);
    cout<<fixed<<setprecision(9)<<rock<<" "<<scissor<<" "<<paper<<"\n";
    return 0;
}
