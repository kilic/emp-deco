
#include <stdint.h>
#include "emp-deco/debug.h"
#include "emp-deco/tls.h"

using namespace std;

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
