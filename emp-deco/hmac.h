#ifndef EMP_DECO_HMAC_H_
#define EMP_DECO_HMAC_H_
#include "emp-sh2pc/emp-sh2pc.h"
#include "utils.h"
#include "sha256.h"
#include <iostream>
using namespace emp;
using namespace std;

wide pad_seed(wide &seed, int seed_bit_len);

class HMAC
{

private:
  wide ipad_start;
  wide opad_start;
  wide ipad;
  wide opad;

  sha256 hasher;

public:
  HMAC(sha256 hasher)
  {
    this->ipad_start = hex_to_emp_word(
        "36363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636", PUBLIC);
    this->opad_start = hex_to_emp_word(
        "5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c", PUBLIC);
    if (!hasher.prepared)
      hasher.prepare_2pc();
    this->hasher = hasher;
  };
  void set_secret(wide secret, int len);
  vector<wide> run(int t, wide seed, int seed_bit_len);
  static wide pad_block_input(wide seed, int seed_bit_len);

private:
  void inner_hmac(wide &out, wide in);
  void outer_hmac(wide &out, wide chain);
};
#endif
