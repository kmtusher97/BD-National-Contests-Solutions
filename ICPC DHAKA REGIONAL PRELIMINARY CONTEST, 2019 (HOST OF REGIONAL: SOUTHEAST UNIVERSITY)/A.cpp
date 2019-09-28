/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll base1 = 191, base2 = 307;
const ll mod1 = 1000001011, mod2 = 1000009999;
vector<ll> power1, power2;

void init( int n ) {
	power1.resize(n);
	power2.resize(n);
    power1[0] = power2[0] = 1;
    for(int i = 1; i < n; i++) {
        power1[i] = (power1[i - 1] * base1) % mod1;
        power2[i] = (power2[i - 1] * base2) % mod2;
    }
}

class PolynomialHash {
public:
    vector< pair<ll, ll> > hh;

    void generateHash( string& s ) {
        int ln = s.length();
        hh.resize(ln);
        ll h1 = 0, h2 = 0;
        for(int i = 0; i < ln; i++) {
            h1 = (h1 * base1 + s[i]) % mod1;
            h2 = (h2 * base2 + s[i]) % mod2;
            hh[i] = {h1, h2};
        }
    }
    pair<ll, ll> getHash( int l, int r ) {
    	ll h1 = hh[r].first;
    	ll h2 = hh[r].second;
    	if( l == 0 ) return {h1, h2};
    	h1 = (h1 - (hh[l - 1].first * power1[r - l + 1]) % mod1 + mod1) % mod1;
    	h2 = (h2 - (hh[l - 1].second * power2[r - l + 1]) % mod2 + mod2) % mod2;
    	return {h1, h2};
    }
};

int main() {
	ios_base :: sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	init( 1200006 );
	int cases, caseno = 0;
	cin >> cases;
	while( cases-- ) {
		string T, P;
		int K;
		cin >> T >> P >> K;
		PolynomialHash hashT, hashP;
		hashT.generateHash( T );
		hashP.generateHash( P );
		int n = T.length();
		int m = P.length();
		ll res = 0;
		for(int i = 0; i <= n - m; i++) {
			int cnt = 0;
			for(int j = i, k = 0; k < m and cnt <= K; j++, k++) {
				if( T[j] != P[k] ) {
					cnt++;
					continue;
				}
				int lo = j, hi = i + m - 1;
				while( lo <= hi ) {
					int mid = lo + (hi - lo) / 2;
					auto ht = hashT.getHash( j, mid );
					auto hp = hashP.getHash( k, k + mid - j );
					if( ht == hp ) lo = mid + 1;
					else           hi = mid - 1;
				}
				k += (lo - j - 1);
				j = lo - 1;
			}
			res += (cnt <= K);
		}
		cout << "Case " << ++caseno << ": " << res << "\n";
	}
}
