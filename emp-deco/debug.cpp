#include "debug.h"
using namespace std;
using namespace emp;

string bin_to_hex(string &s)
{
  string out;
  for (uint i = 0; i < s.size(); i += 4)
  {
    int8_t n = 0;
    for (uint j = i; j < i + 4; ++j)
    {
      n <<= 1;
      if (s[j] == '1')
        n |= 1;
    }
    if (n <= 9)
      out.push_back('0' + n);
    else
      out.push_back('a' + n - 10);
  }

  return out;
}

string bin_to_hex_reversed(string &s)
{
  reverse_str(s);
  return bin_to_hex(s);
}

template <>
void debug<string>(string mark, string desc)
{
  cout << desc << " " << mark << endl;
}

template <>
void debug<char const *>(char const *mark, string desc)
{
  cout << desc << " " << mark << endl;
}

template <>
void debug<int>(int a, string desc)
{
  cout << desc << " " << a << endl;
}

template <>
void debug<Integer>(Integer a, string desc)
{
  string debug = a.reveal<string>(PUBLIC);
  cout << desc << " " << bin_to_hex_reversed(debug) << endl;
}

template <>
void debug<wide>(wide a, string desc)
{
  if (desc != "")
    cout << desc << endl;
  for (int i = 0; i < a.size(); i++)
  {
    string debug = a[i].reveal<string>(PUBLIC);
    cout << bin_to_hex_reversed(debug);
  }
  cout << endl;
}

template <>
void debug<vector<uint32_t>>(vector<uint32_t> a, string desc)
{
  if (desc != "")
    cout << desc << endl;
  for (int i = 0; i < a.size(); i++)
    printf("%8.8x", a[i]);
  printf("\n");
}
