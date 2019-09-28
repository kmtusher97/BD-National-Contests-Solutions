/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;

struct team {
	int solve, penalty, extra;
	vector<int> frozen;
};

int main() {
	ios_base :: sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	int cases, caseno = 0;
	cin >> cases;
	while( cases-- ) {
		int n;
		cin >> n;
		vector<team> teams(n + 1);
		for(int i = 0, id, s, p, m; i < n; i++) {
			cin >> id >> s >> p >> m;
			teams[id].solve = s;
			teams[id].penalty = p;
			teams[id].extra = m;
			teams[id].frozen.resize(m);
			for(auto& p : teams[id].frozen) {
				cin >> p;
			}
		}
		vector<int> rnk(n);
		for(auto& r : rnk) {
			cin >> r;
		}
		bool flg = true;
		vector<team> tmp;
		for(auto& id : rnk) {
			if( tmp.empty() ) {
				for(auto& p : teams[id].frozen) {
					teams[id].solve++;
					teams[id].penalty += p;
				}
				tmp.push_back(teams[id]);
				continue;
			}
			if( teams[id].solve > tmp.back().solve ) {
				flg = false;
				break;
			}
			if( teams[id].solve == tmp.back().solve and teams[id].penalty < tmp.back().penalty ) {
				flg = false;
				break;
			}
			for(auto& p : teams[id].frozen) {
				if( teams[id].solve + 1 < tmp.back().solve ) {
					teams[id].solve++;
					teams[id].penalty += p;
				}
				else if( teams[id].solve + 1 == tmp.back().solve and teams[id].penalty + p >= tmp.back().penalty ) {
					teams[id].solve++;
					teams[id].penalty += p;
				}
				else break;
			}
			tmp.push_back(teams[id]);
		}
		cout << "Case " << ++caseno << ": " << (flg ? "We respect our judges :)\n" : "Say no to rumour >:\n");
	}
}
