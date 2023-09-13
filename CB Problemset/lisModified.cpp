#include <bits/stdc++.h>
#include<stdio.h>
using namespace std;
#define F(i,a,b) for(ll i = a; i <= b; i++)
#define RF(i,a,b) for(ll i = a; i >= b; i--)
#define pii pair<ll,ll>
#define PI 3.14159265358979323846264338327950288
#define ll long long
#define ff first
#define ss second
#define pb(x) push_back(x)
#define mp(x,y) make_pair(x,y)
#define debug(x) cout << #x << " = " << x << endl
#define INF 1000000009
#define mod 1000000007
#define S(x) scanf("%d",&x)
#define S2(x,y) scanf("%d%d",&x,&y)
#define P(x) printf("%d\n",x)
#define all(v) v.begin(),v.end()

ll BIT[200005],arr[200005],wt[200005];

void update(ll id,ll val) {
    for(ll i=id;i<=200000;i+=i&-i) BIT[i]=max(BIT[i],val);
}
ll query(ll id) {
    ll sum=0;
    for(ll i=id;i>0;i-=i&-i) sum=max(BIT[i],sum);
    return sum;
}
int main() 
{
    std::ios::sync_with_stdio(false);
    ll t;
    cin>>t;
    while(t--)
    {
        set <ll> myset; // for co-ordinate compression
        myset.clear();
        map <ll,ll> mymap; // value mapped to its index in range [1,n]
        mymap.clear();
        memset(BIT,0,sizeof(BIT));
        ll n;
        cin>>n;
        ll st = 1; // 1-indexed sorted compressed values
        F(i,0,n-1)
        {
            cin>>arr[i];
            myset.insert(arr[i]);
        }
        F(i,0,n-1)
            cin>>wt[i];
        for(set<ll>::iterator it = myset.begin();it!=myset.end();++it)
            mymap[*it] = st++;
        st--; // compressed values are indexed from [1,st]
        ll ans = 0ll;
        F(i,0,n-1)
        {
            ll index = mymap[arr[i]];
            ll val1 = query(index-1) + wt[i];
            ll val2 = query(index);
            if(val1 > val2)
            {
                ans = max(ans,val1);
                update(index,val1);
            }
        }
        cout<<ans<<endl;
    }
    return 0;
}