/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

typedef tree<
		int,
		null_type,
		less_equal<int>,
		rb_tree_tag,
		tree_order_statistics_node_update
	> ordered_multiset;

struct UnionFind {
	int n;
	vector<int> par;
	UnionFind( int n ) : n(n) {
		par.resize(n + 2);
		for(int i = 0; i <= n; par[i] = i, i++);
	}
	int find( int u ) {
		if( u != par[u] ) par[u] = find( par[u] );
		return par[u];
	}
	void join( int u, int v ) { /**join u with v*/
		par[find( u )] = find( v );
	}
};


int main() {
	ios_base :: sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	int cases, caseno = 0;
	cin >> cases;
	while( cases-- ) {
		int n, Q;
		cin >> n >> Q;
		UnionFind UF(n);
		ordered_multiset tt[n + 2];
		for(int i = 1; i <= n; i++) {
			tt[i].insert(0);
			tt[i].insert(INT_MAX);
		}
		auto join = [&]( int u, int v ) {
			for(auto x : tt[u]) {
				if( !x or x == INT_MAX ) continue;
				tt[v].insert(x);
			}
			tt[u].clear();
			return;
		};
		cout << "Case " << ++caseno << ":\n";
		while( Q-- ) {
			int t, u, v;
			cin >> t >> u >> v;
			if( t == 0 ) {
				int pu = UF.find( u );
				int pv = UF.find( v );
				if( pu == pv ) continue;

				if( tt[pu].size() < tt[pv].size() ) {
					UF.join( u, v );
					join( pu, pv );
				}
				else {
					UF.join( v, u );
					join( pv, pu );
				}
			}
			else if( t == 1 ) {
				int pu = UF.find( u );
				tt[pu].insert( v );
			}
			else {
				int r, l = v;
				cin >> r;
				int pu = UF.find( u );
				int L = tt[pu].order_of_key(*tt[pu].lower_bound(l - 1));
				int R = tt[pu].order_of_key(*tt[pu].lower_bound(r));
				cout << (R - L) << "\n";
			}
		}
	}
}
