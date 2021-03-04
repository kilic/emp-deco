#include "emp-sh2pc/emp-sh2pc.h"
#include <iostream>
#include "utils.h"
#include "hmac.h"
using namespace std;
using namespace emp;

Integer xor_secret(Integer &pad, Integer &secret)
{
  Integer res(512, 0, XOR);
  int off = 512 - secret.size();
  copy_int(res, pad, 0, 0, 512);
  for (int i = 0; i < secret.size(); i++)
  {
    res[i + off] = pad[i + off] ^ secret[i];
  }
  return res;
}

Integer pad_int(int len)
{
  int pad_int_len = 512 - (len % 512);
  Integer pad(pad_int_len, len, PUBLIC);
  pad[len - 1] = 1;
  return pad;
}

HMAC::HMAC(BristolFashion hasher) : hasher(hasher)
{
}

void HMAC::set_secret(Integer &secret)
{
  ipad = xor_secret(ipad_start, secret);
  opad = xor_secret(opad_start, secret);
}

vector<Integer> HMAC::run(int t, Integer seed)
{

  Integer empty(0, 0, PUBLIC);
  vector<Integer> A;
  for (int i = 0; i < t; i++)
  {
    // * a_i can be public
    // * after calculating a_1 in 2PC
    //	remaining a_i can be calculated locally by ALICE
    Integer state(256, 0, PUBLIC);
    if (i == 0)
    {
      state = this->inner_hmac(seed, empty);
    }
    else
    {
      state = inner_hmac(A[i - 1], empty);
    }
    state = outer_hmac(state);
    A.push_back(state);
  }
  vector<Integer> U;
  for (int i = 0; i < t; i++)
  {
    Integer state = inner_hmac(seed, A[i]);
    state = outer_hmac(state);
    U.push_back(state);
  }
  return U;
}

Integer HMAC::inner_hmac(Integer &seed, Integer &chain)
{

  int block_size = this->ipad.size() + seed.size() + chain.size();
  Integer end_pad = pad_int(block_size);
  bool has_chain = chain.size() != 0;

  // key = (premaster_secret^ipad)<64>
  // hash_1 = hash(ipad<64>, seed<_>)

  // * input: xor
  // * intermadiate compression results: public
  // * output: public

  // inner compression:
  // in 2PC
  // inp_1 = [iv<32>, ipad<64>]
  Integer inp_inner(512 + 256, 0, PUBLIC);
  Integer out(256, 0, PUBLIC);
  copy_int(inp_inner, this->iv, 512, 0, 256);
  // TODO: copying public data into xor?
  copy_int(inp_inner, this->ipad, 0, 0, 512);
  hasher.compute(out.bits.data(), inp_inner.bits.data());

  Integer inp(512 + 256, 0, PUBLIC);
  int z = block_size / 512;
  int pad_size = block_size % 512;
  int seed_off = seed.size();
  if (pad_size != 0)
  {
    z -= 1;
  }

  // outer compressions:
  // TODO: can be ALICE only computation
  for (int i = 0; i < z; i++)
  {
    // new_iv<32> = out<32>
    // inp = [new_iv<32>, seed[_:_]<_>, pad<_>]
    Integer inp(512 + 256, 0, PUBLIC);
    copy_int(inp, out, 512, 0, 256);
    if (i == 0 && has_chain)
    {
      seed_off -= 256;
      copy_int(inp, chain, 256, 0, 256);
      copy_int(inp, seed, 0, seed_off, 256);
    }
    else
    {
      seed_off -= 512;
      copy_int(inp, seed, 0, seed_off, 512);
    }
    // cout << "loop " << seed_off << endl;
    hasher.compute(out.bits.data(), inp.bits.data());
  }

  // ALICE only
  if (pad_size != 0)
  {
    // new_iv<32> = out<32>
    // inp = [new_iv<32>, seed[_:]<_>, pad<_>]
    copy_int(inp, out, 512, 0, 256);
    copy_int(inp, seed, end_pad.size(), 0, seed_off);
    copy_int(inp, end_pad, 0, 0, end_pad.size());
    hasher.compute(out.bits.data(), inp.bits.data());
  }

  return out;
}

Integer HMAC::outer_hmac(Integer &chain)
{

  // TODO: we arrive at same intermadiate compression result
  //	many times. we may want to consider store and reuse this value

  // * input: xor
  // * intermadiate compression result: public
  // * output: xor

  // inner compression:
  // inp = [iv<32>, opad_premaster_secret<64>]
  Integer inp(512 + 256, 0, PUBLIC);
  Integer out_1(256, 0, PUBLIC);
  copy_int(inp, this->iv, 512, 0, 256);
  copy_int(inp, this->opad, 0, 0, 512);
  this->hasher.compute(out_1.bits.data(), inp.bits.data());

  // outer compression:
  // new_iv<32> = out_1<32>
  // inp = [new_iv<32>, chain<32>, pad<32>]
  Integer inp_2(512 + 256, 0, PUBLIC);
  Integer out_2(256, 0, PUBLIC);
  copy_int(inp_2, out_1, 512, 0, 256);
  copy_int(inp_2, chain, 256, 0, 256);
  copy_int(inp_2, this->outer_end_pad, 0, 0, 256);
  this->hasher.compute(out_2.bits.data(), inp_2.bits.data());

  return out_2;
}
