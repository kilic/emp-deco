
#include <stdint.h>
#include "emp-deco/debug.h"
#include "emp-deco/tls.h"

using namespace std;

// vector<Integer> to_word(Integer a, int party)
// {
//   int n = a.size() / 32;
//   if (a.size() % 32 != 0)
//     n += 1;

//   vector<Integer> out(n);
//   for (int i = 0; i < n; i++)
//   {
//     out[n - i - 1] = Integer(32, 0, party);
//     cpy_emp_int(out[n - i - 1], a, 0, i * 32, 32);
//   }
//   return out;
// }

// int main(int argc, char **argv)
// {

//   sha256 hasher = sha256();
//   hasher.prepare_2pc();

//   setup_plain_prot(false, "");

//   int seed_bit_len = seed_hex.size() * 4;

//   vector<Integer> seed = hex_to_emp_word(seed_hex, PUBLIC);
//   vector<Integer> secret = hex_to_emp_word(secret_hex, PUBLIC);

//   HMAC hmac = HMAC(hasher);

//   derive_enc_keys_for_alice(HMAC & hmac, string share, string client_random_hex, string server_random_hex);

//   // auto a = hex_to_emp_int(256, "c19ccf8b1aa56f290aca5d90acc674bbf4e4fcc22713aeece542cd22c22f1f09", PUBLIC);
//   // debug(a);

//   // auto b = hex_to_emp_word("c19ccf8b1aa56f290aca5d90acc674bbf4e4fcc22713aeece542cd22c22f1f09", PUBLIC);
//   // debug(b);

//   // auto c = to_word(a, PUBLIC);
//   // debug(c);

//   finalize_plain_prot();
// }

int main(int argc, char **argv)
{
  string client_random = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  string server_random = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

  int port, party;
  parse_party_and_port(argv, &party, &port);

  NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

  setup_semi_honest(io, party);

  sha256 hasher = sha256();
  hasher.prepare_2pc();
  HMAC hmac = HMAC(hasher);
  string secret;
  if (party == ALICE)
  {
    secret = "1";
  }
  else
  {
    secret = "0";
  }
  vector<Integer> enc_keys = derive_enc_keys_for_alice(hmac, secret, client_random, server_random);

  // expect
  // client enc: bb0941a9c66263c9106bab97169183a4
  // server enc: 30e6464281fd14092b1a30af241007cb

  cout << CircuitExecution::circ_exec->num_and() << endl;
  finalize_semi_honest();
  delete io;
}
