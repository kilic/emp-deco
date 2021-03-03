#include "emp-sh2pc/emp-sh2pc.h"
#include "emp-deco/utils.h"
#include "emp-deco/hmac.h"
#include "emp-deco/tls.h"
using namespace emp;
using namespace std;

const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);

// int main(int argc, char **argv)
// {
//   setup_plain_prot(false, "");
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

  string file = circuit_file_location + "/bristol_fashion/sha256.txt";
  BristolFashion hasher(file.c_str());

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
  debug_int(PUBLIC, enc_keys[0], "client enc key");
  debug_int(PUBLIC, enc_keys[1], "server enc key");

  cout << CircuitExecution::circ_exec->num_and() << endl;
  finalize_semi_honest();
  delete io;
}