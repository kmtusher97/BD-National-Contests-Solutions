/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int mxn = 1e5 + 5;

class ChineseReminderTheorm {
public:
    vector<ll> A, M;

    void clear() {
        A.clear(), M.clear();
    }

    ll extendedEuclid( ll a, ll b, ll& x, ll& y ) {
        if( a == 0 ) {
            x = 0, y = 1;
            return b;
        }
        ll x1, y1;
        ll gcd = extendedEuclid( b % a, a, x1, y1 );

        x = y1 - (b / a) * x1;
        y = x1;
        return gcd;
    }

    pair<ll, ll> solve() {
        if( A.empty() or A.size() != M.size() ) return {-1, -1}; // invalid input

        ll a1 = A[0];
        ll m1 = M[0];
        a1 %= m1;

        for(int i = 1; i < A.size(); i++) {
            ll a2 = A[i];
            ll m2 = M[i];

            ll gcd = __gcd(m1, m2);
            if( a1 % gcd != a2 % gcd ) return {-1, -1}; // conflict in equation

            ll p, q;
            extendedEuclid( m1 / gcd, m2 / gcd, p, q );

            ll mod = (m1 / gcd) * m2;
            ll tmp = ((__int128)a1 * (m2 / gcd) * q + (__int128)a2 * (m1 / gcd) * p) % mod;
            a1 = tmp;
            if( a1 < 0 ) a1 += mod;
            m1 = mod;
        }
        return {a1, m1};
    }
};

int main() {
    ios_base :: sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

	vector<int> primes;
	vector<bool> vis(mxn, false);
    for(int i = 2; i < mxn; i++) {
        if( !vis[i] ) {
            primes.push_back(i);
            for(int j = i + i; j < mxn; vis[j] = true, j += i);
        }
    }

	vector<int> factors[mxn];
    for(int i = 1; i < mxn; i++) {
        int n = i;
        for(auto& p : primes) {
            if( p > n ) break;
            int tmp = 1;
            while( n > 0 and n % p == 0 ) {
                n /= p, tmp *= p;
            }
            if( tmp == 1 ) continue;
            factors[i].push_back(tmp);
        }
        if( n > 1 ) factors[i].push_back(n);
    }

    int cases, caseno = 0;
    cin >> cases;
    while( cases-- ) {
        string N;
        int M;
        cin >> N >> M;

        auto findMod = [&](int x) {
            ll rem = 0, lim = 1e17;
            for(auto d : N) {
                rem = (rem * 10) + (d - '0');
                if( rem >= lim ) {
                    rem %= x;
                }
            }
            return (int)(rem % x);
        };

        vector<int> res(M + 2, -1);
        res[0] = res[1] = 0;
        for(auto p : primes) {
            if( p > M ) break;
            ll maxP = p;
            while( p * maxP <= M ) {
            	maxP *= p;
            }
            res[maxP] = findMod( maxP );
            for(int i = maxP / p; i >= p; i /= p) {
                res[i] = res[maxP] % i;
            }
        }

        ChineseReminderTheorm crt;
        for(int i = M; i > 1; i--) {
            if( ~res[i] ) continue;
            if( i << 1 <= M ) res[i] = res[i << 1] % i;
            else {
                crt.clear();
                for(auto& f : factors[i]) {
                    crt.M.push_back(f);
                    crt.A.push_back(res[f]);
                }
                res[i] = crt.solve().first;
            }
        }

        ll ans = 0;
        for(int i = 1; i <= M; i++) {
            ans += res[i];
        }
        cout << "Case " << ++caseno << ": " << ans << "\n";
    }
}
