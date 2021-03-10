#include "emp-sh2pc/emp-sh2pc.h"
#include "hmac.h"

void HMAC::set_secret(wide secret, int secret_len)
{
  // Assume secret bit len is 32*k
  // TODO: assert ipad.size >= secret_len
  int n = ipad_start.size();
  this->ipad.clear();
  this->opad.clear();
  for (int i = 0; i < n; i++)
  {
    if (i < secret_len)
    {
      this->ipad.push_back(this->ipad_start[i] ^ secret[i]);
      this->opad.push_back(this->opad_start[i] ^ secret[i]);
    }
    else
    {
      this->ipad.push_back(this->ipad_start[i]);
      this->opad.push_back(this->opad_start[i]);
    }
  }
}

wide HMAC::pad_block_input(wide seed, int seed_bit_len)
{
  // TODO: assert seed bit len
  int z = seed_bit_len % 512;
  int n_seed = seed_bit_len / 32;
  if (seed_bit_len % 32 != 0)
  {
    n_seed += 1;
  }
  int number_of_pad_int = (512 - z) / 32;

  wide pad;
  bool alter_seed = z % 32 != 0;

  for (int i = 0; i < seed.size() - 1; i++)
    pad.push_back(seed[i]);

  if (alter_seed)
    pad.push_back(seed[n_seed - 1] ^ Integer(32, 1 << (((32 - z) % 32) - 1), PUBLIC)); // TODO: just flip a single bit
  else
  {
    pad.push_back(seed[n_seed - 1]);
  }

  for (int i = 0; i < number_of_pad_int; i++)
  {
    int val = i != number_of_pad_int - 1 ? 0 : seed_bit_len + 512; // 512 comes from xpad size
    Integer el(32, val, PUBLIC);
    if (i == 0 && !alter_seed)
      el = el ^ Integer(32, 1 << 31, PUBLIC); // TODO: just flip a single bit
    pad.push_back(el);
  }
  return pad;
}

void HMAC::inner_hmac(wide &out, wide data)
{
  sha256 h = this->hasher;
  // assert `in` is padded to 64 * k
  int k = data.size() / 16; // 64 bytes, 16 uint32_t
  h.block_init(out, this->ipad);
  for (int i = 0; i < k; i++)
  {
    auto in = wide(data.begin() + i * 16, data.begin() + (i + 1) * 16);
    h.block(out, in);
  }
}

void HMAC::outer_hmac(wide &out, wide chain)
{
  sha256 h = this->hasher;
  h.block_init(out, this->opad);
  // assume chain is paddded
  h.block(out, chain);
}

vector<wide> HMAC::run(int t, wide seed, int seed_bit_len)
{

  vector<wide> A;
  auto in = pad_block_input(seed, seed_bit_len);
  wide state(8);

  // a_1 = HMAC(key, seed)
  // a_i = HMAC(key, a_(i-1))
  for (int i = 0; i < t; i++)
  {

    if (i == 0)
      // state = hash(secret^ipad, seed)
      this->inner_hmac(state, in);
    else
      // state = hash(secret^ipad, a_i)
      this->inner_hmac(state, pad_block_input(state, 256));

    // state = hash(secret^opad, state)
    this->outer_hmac(state, pad_block_input(state, 256));

    {

      // prepare for u
      // a_i' =  a_i|seed

      wide a;
      for (int i = 0; i < 8; i++)
        a.push_back(state[i]);
      for (int i = 0; i < seed.size(); i++)
        a.push_back(seed[i]);
      A.push_back(a);
    }
  }

  // u_i = HMAC(key, a_(i+1)|seed)
  vector<wide> U;
  int block_bit_len = seed_bit_len + 256;
  for (int i = 0; i < t; i++)
  {
    wide state(8);
    auto in = pad_block_input(A[i], block_bit_len);
    // state = hash(key, a_i|seed)
    this->inner_hmac(state, in);
    // state = hash(key, state)
    this->outer_hmac(state, pad_block_input(state, 256));
    U.push_back(state);
  }
  return U;
}
