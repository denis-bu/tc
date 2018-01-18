#include <gtest/gtest.h>

#include <iostream>

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <exception>
#include <algorithm>
#include <limits>
#include <utility>
#include <cmath>
#include <queue>
#include <stack>
#include <fstream>
#include <functional>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;
using vll = vector<ll>;
using vs = vector<string>;


#include "tc/matrix.h"
using namespace tc;

using mll = Matrix<ll>;

const int MOD = 1000000007;
const double PI = 3.14159265358979323846;  /* pi */
const string POSSIBLE = "Possible";
const string IMPOSSIBLE = "Impossible";

namespace {
  std::ostream& operator << (std::ostream& os, const std::vector<string>& x) {
    os << "{";
    for (size_t i = 0; i < x.size(); i++) os << x[i] << ", ";
    os << "}";
    return os;
  }
  std::ostream& operator << (std::ostream& os, const vi& x) {
    os << "{";
    for (size_t i = 0; i < x.size(); i++) os << x[i] << ", ";
    os << "}";
    return os;
  }
  int gcd(int a, int b) {
    while (b > 0) {
      int t = a % b;
      a = b;
      b = t;
    }
    return a;
  }
  int lcm(int a, int b) {
    return abs(a * b) / gcd(a, b);
  }

  unordered_map<ull, ll> nck_cache;
  ull nck_key(int n, int k) {
    return ((ull)n << 32) + (ull)k;
  }
  ll nck(int n, int k) {
    if (k >  n)
      throw invalid_argument("nck: k > n");
    if (k == 0)
      return 1;
    if (k > n / 2)
      return nck(n, n - k);
    auto it = nck_cache.find(nck_key(n, k));
    if (it != nck_cache.end()) return it->second;

    ll bc = nck(n - 1, k - 1) * n / k;
    nck_cache[nck_key(n, k)] = bc;
    return bc;
  }

  string getPossible(bool cond) {
    return cond ? POSSIBLE : IMPOSSIBLE;
  }
}


namespace
{

struct StringHalving {
  string lexsmallest(string s) {
    bool let[26] = {};
    char minc = s[0];
    for (char c: s) {
      if (let[c - 'a'])
        return std::string() + minc;
      let[c - 'a'] = true;
      minc = min(c, minc);
    }
    throw std::runtime_error("no dup");
  }
};

struct TurtleGame {
  string getwinner(const vector<string>& b) {
    int maxc = 0;
    int rows = b.size();
    int cols = b[0].size();
    int countBad = 0;
    for (const auto& r : b) {
      for (char c : r) {
        if (c == '#') countBad += 1;
      }
    }

    int countGood = rows*cols - countBad;
    maxc = countGood - rows - cols + 2;

    string win = "Win";
    string lose = "Lose";
    return (maxc % 2 == 0) ? win : lose;
  }
};

struct HeroicScheduled2 {
  long long getcount(vector<int> start, vector<int> finish) {
    return 1ll;
  }
};

}

TEST(srm_726_test, easy)
{

}

TEST(srm_726_test, mid)
{

}

TEST(srm_726_test, hard)
{

}
