/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Team {
    int solve, penalty, mask, maskPending;
    vector<int> pendingPenalty;
    Team() { }
    Team( int m ) {
        solve = penalty = mask = maskPending = 0;
        pendingPenalty.resize(m, 0);
    }
};

const int mod = 1e9 + 7;

int main() {
    ios_base :: sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    int cases, caseno = 0;
    cin >> cases;
    while( cases-- ) {
        int n, m;
        cin >> n >> m;
        vector<Team> teams(n, Team( m ));
        map<string, int> mp;
        string s;
        cin.ignore();
        getline(cin, s);
        getline(cin, s);
        getline(cin, s);
        for(int i = 0; i < n; i++) {
            getline(cin, s);
            for(auto& c : s) {
                if( c == '/' or c == '|' ) c = ' ';
            }
            mp[s.substr(1, 20)] = i; /**team name*/
            stringstream ss(s.substr(22));
            string tmp;
            ss >> tmp;   /**solve*/
            teams[i].solve = atoi(tmp.c_str());
            ss >> tmp;   /**penalty*/
            teams[i].penalty = atoi(tmp.c_str());

            for(int j = 0; j < m; j++) {
                ss >> tmp;
                if( tmp[0] == '0' or tmp[0] == '-' ) {
                    ss >> tmp;
                    continue;
                }
                int attempt = 0, lastSubmission = 0;
                if( tmp[0] == '?' ) {
                    attempt = atoi(tmp.substr(1).c_str());
                    ss >> tmp;
                    lastSubmission = atoi(tmp.c_str());
                    teams[i].maskPending |= (1 << j);
                    teams[i].pendingPenalty[j] = (attempt - 1) * 20 + lastSubmission;
                    continue;
                }
                ss >> tmp;
                teams[i].mask |= (1 << j);
            }
            getline(cin, s);
        }
        getline(cin, s);
        getline(cin, s);
        vector<int> rnk(n, -1);
        for(int i = 0; i < n; i++) {
            getline(cin, s);
            rnk[i] = mp[s.substr(1, 20)];
        }
        getline(cin, s);

        int lim = (1 << m);
        vector< vector< pair<int, int> > > A(n);
        for(int id = 0; id < n; id++) {
            int maskHave = (teams[id].mask | teams[id].maskPending);
            for(int mask = 0; mask < lim; mask++) {
                if( (maskHave | mask) != maskHave ) continue;
                if( (mask & teams[id].mask) != teams[id].mask ) continue;
                int penalty = teams[id].penalty;
                for(int j = 0; j < m; j++) {
                    if( !(mask & (1 << j)) ) continue;
                    penalty += teams[id].pendingPenalty[j];
                }
                A[id].push_back({mask, penalty});
            }
            auto cmp = [](pair<int, int> a, pair<int, int> b) {
                int c1 = __builtin_popcount(a.first);
                int c2 = __builtin_popcount(b.first);
                if( c1 != c2 ) return c1 < c2;
                return a.second > b.second;
            };
            sort(A[id].begin(), A[id].end(), cmp);
        }

        vector< vector<int> > dp(n);
        for(int i = 0; i < n; i++) {
            dp[i].resize(lim, 0);
        }
        /**base case*/
        for(int i = 0; i < A[rnk[n - 1]].size(); i++) {
            auto tmp1 = A[rnk[n - 1]][i];
            dp[n - 1][tmp1.first] = i + 1;
        }
        for(int i = n - 2; i >= 0; i--) {
            for(int j = 0; j < A[rnk[i]].size(); j++) {
                auto a1 = A[rnk[i]][j];
                int mask1 = a1.first;
                int solveI = __builtin_popcount(mask1);
                int lo = 0, hi = A[rnk[i + 1]].size() - 1, mask2 = -1;
                while( lo <= hi ) {
                    int mid = lo + (hi - lo) / 2;
                    auto tmp = A[rnk[i + 1]][mid];
                    int solveJ = __builtin_popcount(tmp.first);
                    if( solveI > solveJ ) {
                        mask2 = tmp.first;
                        lo = mid + 1;
                    }
                    else if( solveI == solveJ and a1.second <= tmp.second ) {
                        mask2 = tmp.first;
                        lo = mid + 1;
                    }
                    else hi = mid - 1;
                }
                if( mask2 == -1 ) continue;
                dp[i][mask1] = dp[i + 1][mask2] % mod;
                if( !j ) continue;
                (dp[i][mask1] += dp[i][A[rnk[i]][j - 1].first]) %= mod;
            }
        }
        int res = dp[0][A[rnk[0]].back().first];
        cout << "Case " << ++caseno << ": " << res << "\n";
    }
}
/*

1
3 4
|--------------------|-------|-------|-------|-------|-------|
|Team Name           |Total  |A      |B      |C      |D      |
|--------------------|-------|-------|-------|-------|-------|
|Team X              |2/330  |?10/255|0/--   |1/10   |5/240  |
|--------------------|-------|-------|-------|-------|-------|
|Team Y              |2/330  |5/230  |0/--   |1/20   |?5/270 |
|--------------------|-------|-------|-------|-------|-------|
|Team Z              |1/30   |-4/--  |1/30   |?1/241 |?1/245 |
|--------------------|-------|-------|-------|-------|-------|

|--------------------|
|Team Z              |
|Team X              |
|Team Y              |
|--------------------|
*/


