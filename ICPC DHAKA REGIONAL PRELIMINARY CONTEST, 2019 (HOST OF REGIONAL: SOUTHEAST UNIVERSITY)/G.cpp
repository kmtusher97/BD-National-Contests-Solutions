/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int mxn = 1e7 + 7;

int main() {
	ios_base :: sync_with_stdio(0);
	cin.tie(0), cout.tie(0);

	/**Eular Totient*/
	vector<ll> phi(mxn);
	for(int i = 0; i < mxn; i++) {
		phi[i] = i;
	}
	vector<bool> vis(mxn, false);
	vis[1] = true;
	for(int i = 2; i < mxn; i += 2) {
		if( i != 2 ) vis[i] = true;
		phi[i] = phi[i] / 2;
	}

	for(ll i = 3; i < mxn; i += 2) {
		if( vis[i] ) continue;
		phi[i] = phi[i] - 1;
		for(ll j = i + i; j < mxn; j += i) {
			vis[j] = true;
			phi[j] = phi[j] / i * (i - 1);
		}
	}

	for(int i = 1; i < mxn; i++) {
		phi[i] += phi[i - 1];
	}

	int cases, caseno = 0;
	cin >> cases;
	while( cases-- ) {
		ll N, P;
		cin >> N >> P;
		ll lo = 1, hi = N, res = -1;
		while( lo <= hi ) {
			ll mid = (lo + hi) / 2;
			ll nth = N / mid;
			if( phi[nth] >= P ) {
				res = max(res, mid);
				lo = mid + 1;
			}
			else hi = mid - 1;
		}
		cout << "Case " << ++caseno << ": " << res << "\n";
	}
}
