#include<bits/stdc++.h>
#define ll long long
#define pb push_back
#define inf (1LL<<60)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)
using namespace std;

class segtree
{
    ll *tree;
    ll *upd;
    int n;
public:
    segtree(int n)
    {
        this->n = n;
        tree = new ll[4*n];
        upd = new ll[4*n];
        for(int i=0;i<4*n;i++)
            tree[i] = upd[i] = 0;
    }
    void push(int node)
    {
        tree[2*node+1] += upd[node];
        tree[2*node+2] += upd[node];
        upd[2*node+1] += upd[node];
        upd[2*node+2] += upd[node];
        upd[node] = 0;
    }
    void update(int li, int ri, ll val)
    {
        lazyProp(0,0,n-1,li,ri,val);
    }
    void lazyProp(int node, int ln, int rn, int li, int ri, ll val)
    {
        if(ri < ln || rn < li)
            return;
        if(li <= ln && rn <= ri)
        {
            tree[node] += val;
            upd[node] += val;
            return;
        }
        int mid = (ln + rn)/2;
        push(node);
        lazyProp(2*node + 1, ln, mid, li, ri, val);
        lazyProp(2*node + 2, mid + 1, rn, li, ri, val);
        tree[node] = max(tree[2*node + 1], tree[2*node + 2]);
    }
    ll query(int li, int ri, int node = 0, int ln = 0, int rn = - 1)
    {
        rn = (rn == -1)?n-1:rn;
        if(ri < ln || rn < li)
            return -(inf);
        if(li <= ln && rn <= ri)
        {
            return tree[node];
        }
        int mid = (ln+rn)/2;
        push(node);
        ll v1 = query(li, ri, 2*node+1, ln, mid);
        ll v2 = query(li, ri, 2*node+2, mid+1, rn);
        return max(v1, v2);
    }
};

struct interval
{
    ll score;
    int open;
    int close;
};

ll solve(vector<interval>& v, int n)
{
        segtree dp(n);
        vector<int> endings[n];
        for(int i = 0; i < v.size(); i++)
            endings[v[i].close]  .push_back(i);
        for(int i : endings[0])
        {
            dp.update(v[i].open, v[i].close, v[i].score);
        }
        for(int i = 1; i < n; i++)
        {
             ll large = max(dp.query(0, i-1), 0LL);
             dp.update(i,i,large);
             for(int j : endings[i])
                dp.update(v[j].open, v[j].close, v[j].score);
        }
        return max(0LL,dp.query(0, n-1));
}

int main() {
   int n,m; 
   cin >> n >>m;
   vector<interval> v(m);
   for(int i = 0; i < m; i++)
    {
        cin >> v[i].open >> v[i].close >> v[i].score;
        v[i].open--;
        v[i].close--;
    }
   cout << solve(v, n);
   return 0;
}
