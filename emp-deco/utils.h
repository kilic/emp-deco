#ifndef UTILS_H_
#define UTILS_H_
#include "emp-sh2pc/emp-sh2pc.h"

using namespace emp;
using std::string;

const char *hex_char_to_bin(char c);
string hex_to_binary(string hex);
void reverse_str(string &str);
Integer hex_to_integer(int len, string hex_input, int party);
string bin_to_hex(string &s);
string bin_to_hex_reversed(string &s);
void debug_int(int party, Integer a, string desc);
void copy_int(Integer &dst, Integer &src, size_t offset_dst, size_t offset_src, size_t len);

#endif

