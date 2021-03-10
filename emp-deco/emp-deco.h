#ifndef EMP_DECO_EMP_DECO_H_
#define EMP_DECO_EMP_DECO_H_
#include "emp-sh2pc/emp-sh2pc.h"
using namespace std;
using namespace emp;

const int word_size = 32;

typedef vector<Integer> wide;

inline Integer new_emp_int(int party, uint32_t value = 0)
{
  return Integer(word_size, 0, party);
}

inline Integer new_emp_int(uint32_t value = 0)
{
  return Integer(word_size, 0, PUBLIC);
}
#endif