/* BISMILLAHIR RAHMANIR RAHIM */
#include <bits/stdc++.h>
using namespace std;
const int mxn = 1e5 + 5;

namespace SuffixArray {
  char T[mxn];
  int n, c[mxn], ID[mxn];
  int RA[mxn], tempRA[mxn];
  int SA[mxn], tempSA[mxn];

  void init( char* t ) {
    n = strlen(t);
    for(int i = 0; i < n; i++) T[i] = t[i];
    T[n++] = '#', T[n] = '\0';
  }

  void countingSort( int k ) {
    memset(c, 0, sizeof c);
    for(int i = 0; i < n; i++)
      c[i + k < n ? RA[i + k] : 0]++;
    for(int i = 0, sum = 0, lim = max(300, n); i < lim; i++) {
      int t = c[i]; c[i] = sum, sum += t;
    }
    for(int i = 0; i < n; i++)
      tempSA[c[SA[i] + k < n ? RA[SA[i] + k] : 0]++] = SA[i];
    for(int i = 0; i < n; i++) SA[i] = tempSA[i];
  }

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
    for(int i = 0; i < n; i++) ID[SA[i]] = i;
  }

  const int mxlg = 18;
  int phi[mxn], pLCP[mxn], LCP[mxlg][mxn];

  void computeLCP() {
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
    for(int i = 1; i < n; i++)
      LCP[0][i] = pLCP[SA[i]];
    for(int i = 1; i < mxlg; i++) {
      for(int j = 1; j < n; j++) {
        LCP[i][j] = LCP[i - 1][j];
        if( j + (1 << (i - 1)) < n )
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

namespace Persistent {
  struct Node {
    int sum, lft, rgt;
    Node() { sum = lft = rgt = 0; }
  };

  int avail;
  Node tree[mxn * 30];

  void init() {
    avail = 0;
    for(int i = 0; i < mxn * 30; i++) tree[i] = Node();
  }

  int add( int last, int l, int r, int pos ) {
    int cur = ++avail;
    tree[cur] = tree[last];
    if( l == r ) {
      tree[cur].sum++;
      return cur;
    }
    int mid = l + (r - l) / 2;
    if( pos <= mid )  tree[cur].lft = add( tree[last].lft, l, mid, pos );
    else              tree[cur].rgt = add( tree[last].rgt, mid + 1, r, pos );
    tree[cur].sum = tree[tree[cur].lft].sum + tree[tree[cur].rgt].sum;
    return cur;
  }

  int query( int L, int R, int idlft, int idrgt, int l, int r ) {
    if( l > R or r < L or L > R ) return 0;
    if( l >= L and r <= R ) return tree[idrgt].sum - tree[idlft].sum;
    int mid = l + (r - l) / 2;
    return query( L, R, tree[idlft].lft, tree[idrgt].lft, l, mid ) +
           query( L, R, tree[idlft].rgt, tree[idrgt].rgt, mid + 1, r );
  }
};

char s[mxn];
int roots[mxn];

int main() {
  ios_base :: sync_with_stdio(0);
  cin.tie(0), cout.tie(0);
  int cases;
  cin >> cases;
  while( cases-- ) {
    cin >> s;
    SuffixArray :: init( s );
    SuffixArray :: buildSA();
    SuffixArray :: computeLCP();
//    SuffixArray :: print();
    Persistent :: init();
    int n = SuffixArray :: n;
    for(int i = 0; i < n; i++) {
      roots[i + 1] = Persistent :: add( roots[i], 0, n - 1, SuffixArray :: SA[i] );
    }
    int q;
    cin >> q;
    while( q-- ) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      a--, b--, c--, d--;
      auto r = SuffixArray :: stringMatching( c, d );
      int res = Persistent :: query( a, b - d + c, roots[r.first], roots[r.second + 1], 0, n - 1 );
      cout << res << "\n";
    }
  }
}
