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
    vector<string> a(n);
    for(auto& i : a) cin >> i;
    string s;
    cin >> s;
    int m = s.length();
    cout << "Case " << ++caseno << ":\n";
    bool flg = false;
    for(int i = 0; i < n; i++) {
      int j = 0, k = 0;
      for(int ln = a[i].length(); j < m and k < ln; k++) {
        if( s[j] == a[i][k] ) j++;
      }
      if( j == m ) cout << (flg ? " " : "") << i + 1, flg = true;
    }
    if( !flg ) cout << "No files found!";
    cout << "\n";
  }
}
