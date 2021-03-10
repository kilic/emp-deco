#ifndef EMP_DECO_UTILS_H_
#define EMP_DECO_UTILS_H_
#include "emp-sh2pc/emp-sh2pc.h"
#include <iostream>
#include "debug.h"

using namespace std;
using namespace emp;

const char *hex_char_to_bin(char c);
string hex_to_binary(string hex);
void reverse_str(string &str);

Integer inv_int(Integer in);

Integer hex_to_emp_int(int len, string hex_input, int party);
vector<uint32_t> hex_to_word(string hex);
vector<Integer> hex_to_emp_word(string hex, int party);
void cpy_emp_int(Integer &dst, Integer src, int dst_off, int src_off, int len);

#endif
