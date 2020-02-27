/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
const int mxn = 1e5 + 5;

namespace SuffixArray {
  char* T;
  int n; 
  vector<int> RA, tempRA;
  vector<int> SA, tempSA;

  void init( string& t ) {
    n = t.length();
    T = new char[n + 5];
    for(int i = 0; i < n; i++) T[i] = t[i];
    T[n++] = '#', T[n] = '\0';
    RA.resize(n), tempRA.resize(n);
    SA.resize(n), tempSA.resize(n);
  }

  void countingSort( int k ) {
    vector<int> c(max(300, n), 0);
    for(int i = 0; i < n; i++)
      c[i + k < n ? RA[i + k] : 0]++;
    for(int i = 0, sum = 0, lim = max(300, n); i < lim; i++) {
      int t = c[i]; c[i] = sum, sum += t;
    }
    for(int i = 0; i < n; i++)
      tempSA[c[SA[i] + k < n ? RA[SA[i] + k] : 0]++] = SA[i];
    for(int i = 0; i < n; i++) SA[i] = tempSA[i];
  }

  vector<int> ID;

  void buildSA() {
    for(int i = 0; i < n; i++) SA[i] = i;
    for(int i = 0; i < n; i++) RA[i] = T[i];
    for(int k = 1, r = 0; k < n; k <<= 1) {
      countingSort( k );
      countingSort( 0 );
      r = tempRA[0] = 0;
      for(int i = 1; i < n; i++)
        tempRA[SA[i]] = RA[SA[i]] == RA[SA[i - 1]] and
                        RA[SA[i] + k] == RA[SA[i - 1] + k] ? r : ++r;
      for(int i = 0; i < n; i++) RA[i] = tempRA[i];
      if( RA[SA[n - 1]] == n - 1 ) break;
    }
    ID.resize(n);
    for(int i = 0; i < n; i++) ID[SA[i]] = i;
  }

  vector<int> phi, pLCP;
  vector< vector<int> > LCP;

  void computeLCP() {
    phi.resize(n), pLCP.resize(n);
    phi[SA[0]] = -1;
    for(int i = 1; i < n; i++) phi[SA[i]] = SA[i - 1];
    for(int i = 0, L = 0; i < n; i++) {
      if( phi[i] == -1 ) {
        pLCP[i] = 0;
        continue;
      }
      for(; T[i + L] == T[phi[i] + L]; L++);
      pLCP[i] = L;
      L = max(L - 1, 0);
    }
    const int mxlg = 32 - __builtin_clz(n);
    LCP.resize(mxlg, vector<int>(n + 2, 0));
    for(int i = 1; i < n; i++) {
      LCP[0][i] = pLCP[SA[i]];
    }
    for(int i = 1; i < mxlg; i++) {
      for(int j = 1; j + (1 << (i - 1)) < n; j++) { 
          LCP[i][j] = min(LCP[i - 1][j], LCP[i - 1][j + (1 << (i - 1))]);
      }
    }
  }

  int lcpOf( int i, int j ) {
    i++;
    if( i > j ) return INT_MAX;
    int k = 31 - __builtin_clz(j - i + 1);
    return min(LCP[k][i], LCP[k][j - (1 << k) + 1]);
  }

  void print() {
    for(int i = 0; i < n; i++)
      printf("%2d\t%2d\t%2d\t%s\n", SA[i], ID[i], LCP[0][i], T + SA[i]);
  }

  pair<int, int> stringMatching( int L, int R ) {
    int lo = 1, hi = ID[L], lb = hi, ub;
    while( lo <= hi ) {
      int mid = lo + (hi - lo) / 2;
      if( lcpOf( mid, ID[L] ) >= R - L + 1 )
        hi = mid - 1, lb = min(lb, mid);
      else
        lo = mid + 1;
    }
    lo = ID[L], hi = n - 1, ub = lb;
    while( lo <= hi ) {
      int mid = lo + (hi - lo) / 2;
      if( lcpOf( ID[L], mid ) >= R - L + 1 )
        lo = mid + 1, ub = max(ub, mid);
      else
        hi = mid - 1;
    }
    return {lb, ub};
  }
};

int main() {
  ios_base :: sync_with_stdio(0);
  cin.tie(0), cout.tie(0);
  int cases;
  cin >> cases;
  while( cases-- ) {
    string s;
    cin >> s;
    SuffixArray :: init( s );
    SuffixArray :: buildSA();
    SuffixArray :: computeLCP();
    //SuffixArray :: print();
    int n = SuffixArray :: n;
    int q;
    cin >> q;
    vector< vector<int> > qq(q * 4);
    for(int i = 0, j = 0; i < q; i++) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      a--, b--, c--, d--;
      auto tmp = SuffixArray :: stringMatching( c, d );
      int l = tmp.first, r = tmp.second;
      b -= (d - c);
      qq[j++] = {r, b, 1, i};
      qq[j++] = {l - 1, b, -1, i};
      qq[j++] = {r, a - 1, -1, i};
      qq[j++] = {l - 1, a - 1, 1, i};
    }
    sort(qq.begin(), qq.end());
    vector<int> res(n, 0), tt(n + 5, 0);

    auto update = [&]( int id, int v ) {
      if( id == 0 ) return;
      for(; id < n + 5; id += (id & -id)) tt[id] += v;
    };
    auto query = [&]( int id ) {
      if( id <= 0 ) return 0;
      int ret = 0;
      for(; id > 0; id -= (id & -id)) ret += tt[id];
      return ret;
    };
    for(int i = 0, j = 0; i < n; i++) {
      update( SuffixArray :: SA[i] + 1, 1 );
      for(; j < qq.size() and qq[j][0] == i; j++) 
        res[qq[j][3]] += (qq[j][2] * query( qq[j][1] + 1 ));
    }
    for(int i = 0; i < q; i++) cout << res[i] << "\n";
  }
}
