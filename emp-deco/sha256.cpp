#include "sha256.h"

template <>
Integer inverse<Integer>(Integer a)
{
  Integer res(a);
  for (int i = 0; i < a.size(); ++i)
    res.bits[i] = !res.bits[i];
  return res;
}

template <>
uint32_t inverse<uint32_t>(uint32_t a)
{
  return ~a;
}

template <>
uint32_t sha256::k<uint32_t>(int i)
{
  return K[i];
}

template <>
Integer sha256::k<Integer>(int i)
{
  return this->_k[i];
}

template <>
uint32_t sha256::iv<uint32_t>(int i)
{
  return IV[i];
}

template <>
Integer sha256::iv<Integer>(int i)
{
  return this->_iv[i];
}

sha256::sha256()
{
}

void sha256::prepare_2pc()
{
  for (int i = 0; i < 8; i++)
  {
    this->_iv[i] = Integer(32, IV[i], PUBLIC);
  }
  for (int i = 0; i < 64; i++)
    this->_k[i] = Integer(32, K[i], PUBLIC);

  this->prepared = true;
};
