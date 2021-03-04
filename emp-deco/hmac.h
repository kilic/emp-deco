#ifndef EMP_DECO_HMAC_H_
#define EMP_DECO_HMAC_H_
#include "emp-sh2pc/emp-sh2pc.h"
#include <iostream>
using namespace emp;

Integer xor_secret(Integer &pad, Integer &secret);
Integer pad_int(int len);

class HMAC
{

private:
  Integer iv = hex_to_integer(256, "6A09E667BB67AE853C6EF372A54FF53A510E527F9B05688C1F83D9AB5BE0CD19", PUBLIC);
  Integer ipad_start = hex_to_integer(512, "36363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636", PUBLIC);
  Integer opad_start = hex_to_integer(512, "5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c", PUBLIC);
  Integer outer_end_pad = pad_int(512 + 256);

  Integer ipad;
  Integer opad;

  BristolFashion hasher;

public:
  HMAC(BristolFashion hasher);
  void set_secret(Integer &secret);
  vector<Integer> run(int t, Integer seed);

private:
  Integer inner_hmac(Integer &seed, Integer &chain);
  Integer outer_hmac(Integer &chain);
};
#endif
