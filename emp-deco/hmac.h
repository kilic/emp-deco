#ifndef EMP_DECO_HMAC_H_
#define EMP_DECO_HMAC_H_
#include "emp-sh2pc/emp-sh2pc.h"
#include "utils.h"
#include "sha256.h"
#include <iostream>
using namespace emp;
using namespace std;

vector<Integer> pad_seed(vector<Integer> &seed, int seed_bit_len);

class HMAC
{

private:
  vector<Integer> ipad_start;
  vector<Integer> opad_start;
  vector<Integer> ipad;
  vector<Integer> opad;

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
  void set_secret(vector<Integer> secret, int len);
  vector<vector<Integer>> run(int t, vector<Integer> seed, int seed_bit_len);
  static vector<Integer> pad_block_input(vector<Integer> seed, int seed_bit_len);

private:
  void inner_hmac(vector<Integer> &out, vector<Integer> in);
  void outer_hmac(vector<Integer> &out, vector<Integer> chain);
};
#endif
