/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;

int main() {
  ios_base :: sync_with_stdio(0);
  cin.tie(0), cout.tie(0);
  int cases, caseno = 0;
  cin >> cases;
  while( cases-- ) {
    int n;
    cin >> n;
    vector<int> G[n + 1];
    for(int i = 1, u, v; i < n; i++) {
      cin >> u >> v;
      G[u].push_back(v);
      G[v].push_back(u);
    }
    for(int i = 1; i <= n; i++) {
      sort(G[i].begin(), G[i].end());
    }
    vector<int> a(1, 0), st(n + 1), ed(n + 1);
    function<void(int, int)> dfs = [&](int u, int p) {
      st[u] = INT_MAX;
      for(auto& v : G[u]) {
        if( v == p ) continue;
        dfs( v, u );
        st[u] = min(st[u], st[v]);
      }
      ed[u] = a.size();
      a.push_back(u);
      st[u] = min(st[u], ed[u]);
    };
    dfs( 1, 0 );
    set<int> have;
    for(int i = 0; i <= n + 1; i++) have.insert(i);
    vector<bool> res(n + 1, false);
    int q;
    cin >> q;
    cout << "Case " << ++caseno << ":\n";
    while( q-- ) {
      int t, u, x;
      cin >> t >> u;
      if( t == 1 ) {
        cin >> x;
        while( x > 0 ) {
          auto it = have.find(st[u]);
          if( it != have.end() ) {
            res[a[st[u]]] = true;
            have.erase(it), x--;
            continue;
          }
          it = have.upper_bound(st[u]);
          if( *it > ed[u] ) break;
          res[a[*it]] = true;
          have.erase(it), x--;
        }
      }
      else {
        cout << res[u] << "\n";
      }
    }
  }
}
