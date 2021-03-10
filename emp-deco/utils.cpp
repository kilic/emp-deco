#include "emp-sh2pc/emp-sh2pc.h"
#include <iostream>
#include "utils.h"
using namespace std;
using namespace emp;

void reverse_str(string &str)
{
  int n = str.length();

  for (int i = 0; i < n / 2; i++)
    std::swap(str[i], str[n - i - 1]);
}

const char *hex_char_to_bin(char c)
{
  switch (toupper(c))
  {
  case '0':
    return "0000";
  case '1':
    return "0001";
  case '2':
    return "0010";
  case '3':
    return "0011";
  case '4':
    return "0100";
  case '5':
    return "0101";
  case '6':
    return "0110";
  case '7':
    return "0111";
  case '8':
    return "1000";
  case '9':
    return "1001";
  case 'A':
    return "1010";
  case 'B':
    return "1011";
  case 'C':
    return "1100";
  case 'D':
    return "1101";
  case 'E':
    return "1110";
  case 'F':
    return "1111";
  default:
    return "0";
  }
}

string hex_to_binary(string hex)
{
  string bin;
  for (unsigned i = 0; i != hex.length(); ++i)
    bin += hex_char_to_bin(hex[i]);
  return bin;
}

Integer inv_int(Integer in)
{
  Integer res(in);
  for (int i = 0; i < in.size(); ++i)
    res.bits[i] = !res.bits[i];
  return res;
}

Integer hex_to_emp_int(int len, string hex_input, int party)
{
  string bin_input = hex_to_binary(hex_input);
  reverse_str(bin_input);
  Integer a(len, bin_input, party);
  return a;
}

vector<uint32_t> hex_to_word(string hex)
{
  int n = hex.size() / 8;
  vector<uint32_t> out(n);
  for (int i = 0; i < n; i++)
  {
    string in = hex.substr(hex.size() - (i + 1) * 8, 8);
    out[n - 1 - i] = stoul(in, 0, 16);
  }
  return out;
}

vector<Integer> hex_to_emp_word(string hex, int party)
{

  int n = hex.size() / 8;
  int r = hex.size() % 8;

  if (r != 0)
  {
    n += 1;
  }
  vector<Integer> out;
  for (int i = 0; i < n; i++)
  {

    int len = 8;
    int off = i * len;
    if (i == n - 1 && r != 0)
    {
      len = r;
      string in = hex.substr(off, len);
      cout << r << endl;
      out.push_back(Integer(32, stoul(in, 0, 16) << ((8 - r) * 4), party));
    }
    else
    {
      string in = hex.substr(off, len);
      out.push_back(Integer(32, stoul(in, 0, 16), party));
    }
  }
  return out;
}

void cpy_emp_int(Integer &dst, Integer src, int dst_off, int src_off, int len)
{
  for (int i = 0; i < len; i++)
  {
    dst[i + dst_off] = src[i + src_off];
  }
}