#include <bits/stdc++.h>
using namespace std;
 
const int MAX = 131072; 
const int OO = 0x3f3f3f3f;
const double EPS = 1e-9; 
 
#define bug(x) cout << #x << " = " << x << '\n'
#define FOR(i, a, n) for(int i = a; i < n; i++)
#define REP(i, n) FOR(i, 0, n)
#define fi first
#define se second
#define pb push_back
#define mt make_tuple
#define mp make_pair
#define all(vetor) vetor.begin(), vetor.end()
#define X real()
#define Y imag()
//#define gc getchar_unlocked

typedef long long ll;
typedef long double ld;
//typedef pair<int, int> ii;
//typedef pair<int, ii> iii;
typedef complex<ll> Pll;
typedef complex<ld> Pld;


typedef pair<int, int> edge;

vector<edge> tree[4 * MAX], query[4 * MAX];;
int pai[MAX], sz[MAX];
stack<pair<int, int>> stk, size;
vector<int> ans;

int find(int x)
{
    if(pai[x] == x)
        return x;
    stk.push(mp(x, pai[x]));
    size.push(mp(x, sz[x]));
    return pai[x] = find(pai[x]);
}

void join(int x, int y)
{
    x = find(x);
    y = find(y);
    if(x == y) return;
    if(sz[x] > sz[y])
        swap(x, y);
    stk.push(mp(x, pai[x])); 
    size.push(mp(y, sz[y]));   
    sz[y] += sz[x];
    pai[x] = y;
}
 
void rollback(int rollback_to)//desfaz todas as alterações no DSU, 
{//O(k) onde k é a quantidade de operações realizadas
	while(rollback_to < stk.size())
	{
		pai[stk.top().fi] = stk.top().se;
		stk.pop();	
        sz[size.top().fi] = size.top().se;
        size.pop();
	}
}
 
void add_edge(int node, int start, int end, int l, int r, edge e)
{
    if(start == l and end == r)
    {
        tree[node].push_back(e);
        return;
    }
    if(l >= r)
        return;
    int mid = (start + end) / 2;
    add_edge(2*node, start, mid, l, min(mid, r), e);
    add_edge(2*node + 1, mid + 1, end, max(l, mid + 1), r, e);
}
 
void add_query(int node, int start, int end, int idx, edge e)
{
    if(start == end)
        query[node].push_back(e);
    else
    {
        int mid = (start + end) / 2;
        if(idx <= mid)
            add_query(2*node, start, mid, idx, e);
        else
            add_query(2*node + 1, mid + 1, end, idx, e);
    }
}
 
void processar(int node)
{
    for(auto it : tree[node])
        join(it.first, it.second);
}
 
void dfs(int node, int start, int end)
{
    int rollback_to = stk.size();
    processar(node);
    if(start == end)
    {
        for(auto v : query[node])
        {
            bool rep = (find(v.first) == find(v.second));
            ans.push_back(rep);
        }
    }
    else
    {
        int mid = (start + end) / 2;
        dfs(2*node, start, mid);
        dfs(2*node + 1, mid + 1, end);
    }
    rollback(rollback_to);
}
 
int main()
{	
    int n, q, o, u, v;
 
    cin >> n >> q;
    for(int i = 0; i <= n; i++)
        sz[i] = 1, pai[i] = i;
    int cur = 0;
    map<pair<int, int>, int> mapa;
    while(q--)
    {
        cin >> o >> u >> v; u--; v--;
		if(u > v) swap(u, v);
        if(o == 1)// adicionar aresta
            mapa[mp(u, v)] = cur++;
        else if(o == 2)// remover aresta
        {
            add_edge(1, 0, MAX-1, mapa[mp(u, v)], cur++, mp(u, v));
            mapa.erase(mp(u, v));
        }
        else // verificar se dois vertices estão na mesma componente
            add_query(1, 0, MAX-1, cur++, mp(u, v));
    }
    cur++;
    for(auto it : mapa)
        add_edge(1, 0, MAX-1, it.second, cur, it.first);
    dfs(1, 0, MAX-1);
    for(int i = 0; i < ans.size(); i++)
        cout << (ans[i] ? "Yes\n" : "No\n");
 
    return 0;
}