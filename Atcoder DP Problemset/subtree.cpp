#include<bits/stdc++.h>
#define ll long long
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)
using namespace std;

vector<int> tree[100001];
ll dp1[100001], dp2[100001];
void solve2(int src, bool vis[], int m, ll par_ans)
{
    vis[src] = 1;
    dp2[src] = ((1+par_ans) * dp1[src]) % m;
    vector<ll> prefix, suffix;
    for(int child : tree[src])
    {
        if(!vis[child]){
        prefix.push_back(1 + dp1[child]);
        suffix.push_back(1 + dp1[child]);}
    }
    for(int i = suffix.size() - 2; i >= 0; i --)
    {
        suffix[i] = (suffix[i] * suffix[i+1]) % m;
    }
    for(int i = 1; i < prefix.size(); i ++)
    {
        prefix[i] = (prefix[i] * prefix[i-1]) % m;
    }
    int cno = 0;
    for(int child : tree[src])
    {
        if(!vis[child])
        {
            ll ways = (1 + par_ans);
            ways = (cno > 0) ? (ways * prefix[cno-1]) % m : ways;
            ways = (cno < suffix.size() - 1) ? (ways * suffix[cno+1]) %m : ways;
            solve2(child, vis, m, ways);
            cno++;
        }
    }
}

void solve1(int src, bool vis[], int m)
{
    vis[src] = 1;
    dp1[src] = 1;
    for(int child : tree[src])
    {
        if(!vis[child])
            {
                solve1(child, vis, m);
                dp1[src] = (dp1[src] * (1 + dp1[child]))%m;
            }
    }
}

int main() {
   int n,m; 
   cin >> n >>m;
   for(int i = 0; i < n-1; i++)
   {
       int u,v;
       cin >> u>> v;
       tree[u].push_back(v);
       tree[v].push_back(u);
   }
   bool vis[n+1];
   memset(vis, 0, sizeof vis);
   solve1(1, vis, m);
   memset(vis, 0, sizeof vis);
   solve2(1, vis, m, 0);
   for(int i = 1; i <= n; i++)
    cout<<dp2[i]<<'\n';
   return 0;
}
