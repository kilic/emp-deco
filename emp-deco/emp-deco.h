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

inline wide wide_xor(wide a, wide b)
{
  wide c;
  for (int i = 0; i < min(a.size(), b.size()); i++)
  {
    Integer ci = a[i] ^ b[i];
    c.push_back(ci);
  }
  return c;
}

#endif