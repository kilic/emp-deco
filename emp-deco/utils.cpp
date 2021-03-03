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

Integer hex_to_integer(int len, string hex_input, int party)
{
  string bin_input = hex_to_binary(hex_input);
  reverse_str(bin_input);
  Integer a(len, bin_input, party);
  return a;
}

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

void debug_int(int party, Integer a, string desc)
{

  std::cout << desc << std::endl;
  string debug = a.reveal<string>(party);
  std::cout << bin_to_hex_reversed(debug) << std::endl;
}

void copy_int(Integer &dst, Integer &src, size_t offset_dst, size_t offset_src, size_t len)
{
  for (int i = 0; i < len; i++)
  {
    dst[i + offset_dst] = src[i + offset_src];
  }
}
