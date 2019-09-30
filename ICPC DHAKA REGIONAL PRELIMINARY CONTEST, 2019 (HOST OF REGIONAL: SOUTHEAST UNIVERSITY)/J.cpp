/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
	ios_base :: sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	int cases, caseno = 0;
	cin >> cases;
	while( cases-- ) {
		int n;
		cin >> n;
		vector<ll> D(n);
		for(auto& d : D) {
			cin >> d;
		}
		sort(D.begin(), D.end());
		cout << "Case " << ++caseno << ": " << (D.front() * D.back()) << "\n";
	}
}
